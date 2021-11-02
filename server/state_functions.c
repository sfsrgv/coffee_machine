#include "state_functions.h"
#include "char_reading.h"
#include "chat_functions.h"

extern int state;
extern int buffer_socket_descriptor;
extern int amount_of_water;
extern int amount_of_milk;
extern int amount_of_coffee;

FILE *setting_file;

void enter_turning_on_state() {
    setting_file = fopen("settings.txt", "r");
}

void process_turning_on_event() {
    if (setting_file == NULL)
        return;
    fscanf(setting_file, "%d %d %d", &amount_of_water, &amount_of_coffee, &amount_of_milk);
    printf("water = %d\ncoffee = %d\nmilk == %d\n", amount_of_water, amount_of_coffee, amount_of_milk);
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
    char_auto_ptr message = get_message(buffer_socket_descriptor);
    if (strncmp(message, "RECIPES", 7) == 0)
        state = GETTING_COFFEE_TYPE;
    if (strncmp(message, "CUSTOM", 6) == 0)
        state = WAITING_FOR_RECIPE;
    if (strncmp(message, "OFF", 3) == 0)
        state = TURNING_OFF;
}

void enter_getting_coffee_type_state() {
    send_message(buffer_socket_descriptor, "MENU:\nESPRESSO\nAMERICANO\nCAPPUCCINO\nLATTE\nRAF\n");
}

void process_getting_coffee_type_event() {}

void exit_getting_coffee_type_state() {

}

void enter_waiting_for_recipe_state() {}

void process_waiting_for_recipe_event() {}

void exit_waiting_for_recipe_state() {}

void process_checking_event() {}

void exit_checking_state() {}

void process_making_coffee_event() {}

void exit_making_coffee_state() {}

void process_waiting_for_resources_event() {}

void exit_waiting_for_resources_state() {}

void enter_turning_off_state() {}

void process_turning_off_event() {}

void exit_turning_off_state() {}

void process_off_event() {
    char_auto_ptr message = get_message(buffer_socket_descriptor);
    while (strncmp(message, "ON", 2) != 0) {
        send_message(buffer_socket_descriptor, "COFFEE MACHINE IS OFF");
        message = get_message(buffer_socket_descriptor);
    }
    send_message(buffer_socket_descriptor, "COFFEE MACHINE IS ON!");
}

void exit_off_state() {
    state = TURNING_ON;
}