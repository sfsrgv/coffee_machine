#ifndef CHAT_FUNCTIONS_H
#define CHAT_FUNCTIONS_H

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

#include "char_reading.h"

#define MAX_MESSAGE_LENGTH 10000

enum DIALOG_STATUS {
    CONTINUE,
    END
};

// Returns time in format yyyy.mm.dd hh:mm:ss
char *get_time();

// Sending messages
int send_message(int socket_descriptor, char* message);

// Getting messages
char* get_message(int socket_descriptor);


#endif //CHAT_FUNCTIONS_H
