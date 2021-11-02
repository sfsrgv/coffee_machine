#ifndef CHAR_READING_H
#define CHAR_READING_H

#include <malloc.h>

// Free ptr
void free_char_auto_ptr(char **ptr);

// Char* with automatically freeing memory
#define char_auto_ptr char* __attribute__((cleanup(free_char_auto_ptr)))

// Reads line from console
#define READ_LINE(line)                       \
        do {                                  \
            static size_t length = 0;         \
            getline(&(line), &length, stdin); \
            (line)[strlen(line) - 1] = 0;     \
        } while(0);

#endif //CHAR_READING_H
