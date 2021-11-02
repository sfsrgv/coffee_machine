// Sofa Sergeeva
//
// Create database with server-client system
//
// To compile program use command:
// gcc main.c chat_functions.c char_reading.c command.c database.c -lpthread
//
// This program need to be run after server for correct work

#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <pthread.h>

#include "socket_constants.h"
#include "chat_functions.h"
#include "char_reading.h"
#include "command.h"

int main() {
    srand(time(NULL));

    // Creating client socket
    int client_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (client_descriptor == -1) {
        printf("Error while creating socket\n");
        return 1;
    }

    // Getting all socket info from "socket_constants.h"
    struct sockaddr_in client_info;
    memset(&client_info, 0, sizeof(client_info));
    client_info.sin_family = AF_INET;
    client_info.sin_addr.s_addr = IP_ADDRESS;
    client_info.sin_port = PORT_NUMBER;

    // Trying to connect to server
    if (connect(client_descriptor, (struct sockaddr *) &client_info, sizeof(client_info)) == -1) {
        printf("Error while connecting\n ");
        return 1;
    }
    int j;
    while(1) {
        char_auto_ptr command;
        READ_LINE(command);
        if (command == NULL) {
            printf("ERROR WHILE READING COMMAND\n");
            break;
        }
        if (send_message(client_descriptor, command) == END)
            break;
        if (get_message(client_descriptor) == END)
            break;
    }
    close(client_descriptor);
    return 0;
}

