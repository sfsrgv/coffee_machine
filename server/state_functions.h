#ifndef SERVER_STATE_FUNCTIONS_H
#define SERVER_STATE_FUNCTIONS_H

#include <stdlib.h>

#include "state_functions.h"
#include "char_reading.h"
#include "chat_functions.h"
#include "asprintf.h"

#define NUMBER_OF_STATES        10
#define TURNING_ON              0
#define WAITING_FOR_COMMANDS    1
#define GETTING_COFFEE_TYPE     2
#define CHECKING                3
#define WAITING_FOR_RESOURCES   4
#define MAKING_COFFEE           5
#define WAITING_FOR_RECIPE      6
#define TURNING_OFF             7
#define OFF                     8
#define COUNTING_RESOURCES      9

#define SAFE_RUN(func)                   \
            do {                         \
                if ((func) != NULL)      \
                   func();               \
                if (state == -1)         \
                   break;                \
            } while (0)

#define SAFE_SEND(socket, message)                          \
            do {                                            \
                if (send_message(socket, message) == ERROR) \
                    state = -1;                             \
            } while (0)

#define SAFE_GET(socket, message)                           \
            do {                                            \
                message = get_message(socket);              \
                if (message == NULL)                        \
                    state = -1;                             \
            } while (0)

struct coffee {
    char *name;
    int water;
    int coffee;
    int milk;
};

struct state {
    void (*enter)();
    void (*process)();
    void (*exit)();
};

// Prints name of state number i in state table
void print_state_name (int i);

// Turning on coffee machine and read settings file
void enter_turning_on_state();
void process_turning_on_event();
void exit_turning_on_state();

// Getting command and going to state depends on it
void process_waiting_for_commands_event();
void exit_waiting_for_commands_state();

// Prints menu of coffee and waits for customer to enter coffee name
void enter_getting_coffee_type_state();
void process_getting_coffee_type_event();
void exit_getting_coffee_type_state();

// If customer want to make custom coffee gets amount of coffee, milk and water for it
void enter_waiting_for_recipe_state();
void process_waiting_for_recipe_event();
void exit_waiting_for_recipe_state();

// Checks if coffee machine has enough resources to make chosen coffee
void process_checking_event();
void exit_checking_state();

// Makes chosen coffee
void process_making_coffee_event();
void exit_making_coffee_state();

// Adding resources to coffee machine
void process_waiting_for_resources_event();
void exit_waiting_for_resources_state();

// Turning off coffee machine and printing settings to file
void enter_turning_off_state();
void process_turning_off_event();
void exit_turning_off_state();

// Waiting for turning on command
void process_off_event();
void exit_off_state();

// Prints amount of coffee machine resources
void process_count_resources_event();
void exit_count_resources_state();


#endif //SERVER_STATE_FUNCTIONS_H
