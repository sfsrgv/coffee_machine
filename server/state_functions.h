#ifndef SERVER_STATE_FUNCTIONS_H
#define SERVER_STATE_FUNCTIONS_H

#define NUMBER_OF_STATES 9
#define TURNING_ON 0
#define WAITING_FOR_COMMANDS 1
#define GETTING_COFFEE_TYPE 2
#define CHECKING 3
#define WAITING_FOR_RESOURCES 4
#define MAKING_COFFEE 5
#define WAITING_FOR_RECIPE 6
#define TURNING_OFF 7
#define OFF 8

void enter_turning_on_state();
void process_turning_on_event();
void exit_turning_on_state();

void process_waiting_for_commands_event();
void exit_waiting_for_commands_state();

void enter_getting_coffee_type_state();
void process_getting_coffee_type_event();
void exit_getting_coffee_type_state();

void enter_waiting_for_recipe_state();
void process_waiting_for_recipe_event();
void exit_waiting_for_recipe_state();

void process_checking_event();
void exit_checking_state();

void process_making_coffee_event();
void exit_making_coffee_state();

void process_waiting_for_resources_event();
void exit_waiting_for_resources_state();

void enter_turning_off_state();
void process_turning_off_event();
void exit_turning_off_state();

void process_off_event();
void exit_off_state();

#endif //SERVER_STATE_FUNCTIONS_H
