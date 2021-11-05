// Sofa Sergeeva
//
// Create coffee machine with server-client system
//
// To compile program use command:
// gcc main.c char_reading.c chat_functions.c state_functions.c
//
// This program need to be run before client for correct work

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
int state = OFF;

extern struct state state_table[NUMBER_OF_STATES];

int main() {
    // Creating server socket
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

    // Running coffee machine until error happens
    while (1) {
        print_state_name(state);
        SAFE_RUN(state_table[state].enter);
        SAFE_RUN(state_table[state].process);
        SAFE_RUN(state_table[state].exit);
    }

    printf("ERROR HAPPENED\n");
    close(buffer_socket_descriptor);
    close(server_descriptor);
    return 0;
}