#include <stdlib.h>
#include "state_functions.h"
#include "char_reading.h"
#include "chat_functions.h"
#include "asprintf.h"

extern int state;
extern int buffer_socket_descriptor;
extern int water_in_machine;
extern int milk_in_machine;
extern int coffee_in_machine;

struct coffee recipes[5] = {{"ESPRESSO",   50,  5,  0},
                            {"AMERICANO",  150, 15, 0},
                            {"CAPPUCCINO", 150, 20, 50},
                            {"LATTE",      150, 20, 100},
                            {"RAF",        150, 20, 150}};

int current_water = 0;
int current_milk = 0;
int current_coffee = 0;
int lack_of_resources = 0;

char *message;

FILE *setting_file;

void enter_turning_on_state() {
    setting_file = fopen("settings.txt", "r");
}

void process_turning_on_event() {
    if (setting_file == NULL)
        return;
    fscanf(setting_file, "%d %d %d", &water_in_machine, &coffee_in_machine, &milk_in_machine);
    printf("[water = %dml]\n[coffee = %dg]\n[milk == %dml]\n", water_in_machine, coffee_in_machine, milk_in_machine);
}

void exit_turning_on_state() {
    if (setting_file == NULL)
        state = OFF;
    else {
        state = WAITING_FOR_COMMANDS;
        fclose(setting_file);
    }
}

void process_waiting_for_commands_event() {
    message = get_message(buffer_socket_descriptor);
}

void exit_waiting_for_commands_state() {
    if (strncmp(message, "RECIPES", 7) == 0) {
        state = GETTING_COFFEE_TYPE;
        return;
    }
    if (strncmp(message, "CUSTOM", 6) == 0) {
        state = WAITING_FOR_RECIPE;
        return;
    }
    if (strncmp(message, "OFF", 3) == 0) {
        state = TURNING_OFF;
        return;
    }
    send_message(buffer_socket_descriptor, "Unknown command\n");
    state = WAITING_FOR_COMMANDS;
}

void enter_getting_coffee_type_state() {
    send_message(buffer_socket_descriptor, "MENU:\nESPRESSO\nAMERICANO\nCAPPUCCINO\nLATTE\nRAF\n");
}

void process_getting_coffee_type_event() {
    message = get_message(buffer_socket_descriptor);
    for (int i = 0; i < 5; ++i) {
        if (strncmp(message, recipes[i].name, strlen(recipes[i].name)) == 0) {
            current_water = recipes[i].water;
            current_coffee = recipes[i].coffee;
            current_milk = recipes[i].milk;
            return;
        }
    }
    current_water = -1;
}

void exit_getting_coffee_type_state() {
    if (current_water == -1)
        state = WAITING_FOR_RECIPE;
    else
        state = CHECKING;
}

void enter_waiting_for_recipe_state() {
    send_message(buffer_socket_descriptor, "Enter recipe (amount of water, coffee, milk):\n");
}

void process_waiting_for_recipe_event() {
    message = get_message(buffer_socket_descriptor);
    current_water = atoi(message);
    char *current_space = strchr(message, ' ');
    current_coffee = atoi(current_space);
    current_space = strchr(current_space + 1, ' ');
    current_milk = atoi(current_space);
}

void exit_waiting_for_recipe_state() {
    state = CHECKING;
}

void process_checking_event() {
    lack_of_resources =
            current_coffee > coffee_in_machine || current_milk > milk_in_machine || current_water > water_in_machine;
}

void exit_checking_state() {
    state = lack_of_resources ? WAITING_FOR_RESOURCES : MAKING_COFFEE;
}

void process_making_coffee_event() {
    water_in_machine -= current_water;
    milk_in_machine -= current_milk;
    coffee_in_machine -= current_coffee;
    send_message(buffer_socket_descriptor, "Take your coffee\n");
}

void exit_making_coffee_state() {
    state = WAITING_FOR_COMMANDS;
}

void process_waiting_for_resources_event() {
    message = "";
    asprintf(&message,
             "Not enough resources for this coffee\nYou have: %d ml of water, %d g of coffee, %d ml of milk\nCoffee needs: %d ml of water, %d g of coffee, %d ml of milk\n",
             water_in_machine, coffee_in_machine, milk_in_machine, current_water, current_coffee, current_milk);
    send_message(buffer_socket_descriptor, message);
    message = get_message(buffer_socket_descriptor);
    char *space = strchr(message, ' ');
    int amount = atoi(space);
    if (strncmp(message, "WATER", 5) == 0)
        water_in_machine += amount;
    if (strncmp(message, "COFFEE", 6) == 0)
        coffee_in_machine += amount;
    if (strncmp(message, "MILK", 4) == 0)
        milk_in_machine += amount;
}

void exit_waiting_for_resources_state() {
    state = CHECKING;
}

void enter_turning_off_state() {
    setting_file = fopen("settings.txt", "w");
}

void process_turning_off_event() {
    send_message(buffer_socket_descriptor, "");
    fprintf(setting_file, "%d %d %d", water_in_machine, coffee_in_machine, milk_in_machine);
}

void exit_turning_off_state() {
    fclose(setting_file);
    state = OFF;
}

void process_off_event() {
    message = get_message(buffer_socket_descriptor);
    while (strncmp(message, "ON", 2) != 0) {
        send_message(buffer_socket_descriptor, "COFFEE MACHINE IS OFF");
        message = get_message(buffer_socket_descriptor);
    }
    send_message(buffer_socket_descriptor, "COFFEE MACHINE IS ON!");
}

void exit_off_state() {
    free(message);
    state = TURNING_ON;
}