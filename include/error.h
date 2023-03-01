#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>

typedef struct Error {
    const char *message;
    const char *source;
    void (*print)(struct Error);
} Error;

#define Error(msg) \
    (Error) { .message = msg, .source = __func__, .print = print_error }

static inline void print_error(Error err) {
    printf("Error: %s\n\tIn: %s\n", err.message, err.source);
}

#endif  // ERROR_H_
