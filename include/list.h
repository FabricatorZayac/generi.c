#ifndef LIST_H_
#define LIST_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
    void *(*get)(void *self, size_t index);
    void *(*pop)(void *self);
    void (*push)(void *self, uint8_t *value_bytes);
    void (*put)(void *self, size_t index, uint8_t *value_bytes);
} ListTrait;

#endif // LIST_H_
