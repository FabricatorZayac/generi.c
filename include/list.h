#ifndef LIST_H_
#define LIST_H_

#include <stddef.h>

typedef struct {
    void *(*get)(void *self, size_t index);
    void *(*pop_back)(void *self);
    void (*push_back)(void *self, char *value_bytes);
    void (*put)(void *self, size_t index, char *value_bytes);
} ListTrait;

#endif // LIST_H_
