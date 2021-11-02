#include "char_reading.h"

void free_char_auto_ptr(char **ptr) {
    free(*ptr);
    *ptr = NULL;
}

