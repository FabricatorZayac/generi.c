#ifndef LIST_H_
#define LIST_H_

#include <stddef.h>

typedef struct {
    void *(*get)(void *self, size_t index);
    void (*put)(void *self, size_t index, char *value_bytes);
    void (*push_back)(void *self, char *value_bytes);
    void (*push_front)(void *self, char *value_bytes);
    void *(*pop_back)(void *self);
    void *(*pop_front)(void *self);
    void (*append)(void *self, void *);
    void (*insert)(void *self, size_t index, char *value_bytes);
    void *(*remove)(void *self, size_t index);
} ListTrait;

#endif // LIST_H_
