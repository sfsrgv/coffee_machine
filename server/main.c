// Sofa Sergeeva
//
// Create database with server-client system
//
// To compile program use command:
//
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "chat_functions.h"
#include "socket_constants.h"
#include "state_functions.h"

int buffer_socket_descriptor;
int water_in_machine = 0;
int milk_in_machine = 0;
int coffee_in_machine = 0;

#define SAFE_RUN(func)                   \
            do {                         \
                if ((func) != NULL)      \
                   func();               \
            } while (0)

struct state {
    void (*enter)();

    void (*process)();

    void (*exit)();
};

struct state state_table[NUMBER_OF_STATES] = {
        // TURNING ON STATE
        {
                enter_turning_on_state,
                process_turning_on_event,
                exit_turning_on_state
        },
        // WAITING FOR COMMANDS ON STATE
        {
                NULL,
                process_waiting_for_commands_event,
                exit_waiting_for_commands_state
        },
        // GETTING COFFEE TYPE STATE
        {
                enter_getting_coffee_type_state,
                process_getting_coffee_type_event,
                exit_getting_coffee_type_state
        },
        // CHECKING STATE
        {
                NULL,
                process_checking_event,
                exit_checking_state
        },
        // WAITING FOR RESOURCES STATE
        {
                NULL,
                process_waiting_for_resources_event,
                exit_waiting_for_resources_state
        },
        // MAKING COFFEE
        {
                NULL,
                process_making_coffee_event,
                exit_making_coffee_state
        },
        // WAITING FOR RECIPE
        {
                enter_waiting_for_recipe_state,
                process_waiting_for_recipe_event,
                exit_waiting_for_recipe_state
        },
        // TURNING OFF STATE
        {
                enter_turning_off_state,
                process_turning_off_event,
                exit_turning_off_state
        },
        // OFF STATE
        {
                NULL,
                process_off_event,
                exit_off_state
        }};


int state = OFF;


int main() {
    int server_descriptor = socket(AF_INET, SOCK_STREAM, 0);;
    if (server_descriptor == -1) {
        printf("Error while creating socket");
        return 1;
    }

    struct sockaddr_in server_info;
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = IP_ADDRESS;
    server_info.sin_port = PORT_NUMBER;

    int i = 1;
    while (bind(server_descriptor, (struct sockaddr *) &server_info, sizeof(server_info)) == -1) {
        printf("%d: Error while binding\n", i++);
        sleep(1);
    }

    if (listen(server_descriptor, BACKLOG) == -1) {
        printf("Error while listening");
        return 1;
    }

    struct sockaddr_in client_info;
    memset(&client_info, 0, sizeof(client_info));
    socklen_t socket_size = sizeof(client_info);
    buffer_socket_descriptor = accept(server_descriptor, (struct sockaddr *) &client_info,
                                      &socket_size);
    if (buffer_socket_descriptor == -1) {
        printf("Error in temporary socket creation");
        exit(1);
    }

    while (state != -1) {
        printf("state = %d\n", state);
        SAFE_RUN(state_table[state].enter);
        SAFE_RUN(state_table[state].process);
        SAFE_RUN(state_table[state].exit);
    }
    close(buffer_socket_descriptor);
    close(server_descriptor);
    return 0;
}