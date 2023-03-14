#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "list.h"
#include "macros.h"

/* remove and pop methods return owned values that need to be freed */
#define DefineLinkedList(NewType, T)                          \
    typedef struct TOKENPASTE(node, ADHOC) {                  \
        T body;                                               \
        struct TOKENPASTE(node, ADHOC) * next, *prev;         \
    } NewType##_node;                                         \
    typedef struct TOKENPASTE(linkedlist, ADHOC) {            \
        NewType##_node *head;                                 \
        NewType##_node *tail;                                 \
        size_t size;                                          \
        const size_t element_size;                            \
        const LinkedListTrait impl;                           \
        DERIVE(LIST, struct TOKENPASTE(linkedlist, ADHOC), T) \
    } NewType;                                                \
    PROXY(LIST, NewType, T)                                   \
    NewType NewType##_new() {                                 \
        return (NewType) {.element_size = sizeof(T),          \
                          .head = NULL,                       \
                          .size = 0,                          \
                          .impl = LinkedListImpl,             \
                          PROXY_ASSIGN(LIST, NewType)};       \
    }

typedef struct {
    DERIVE_TRAIT(LIST)
    void (*destroy)(void *self);
} LinkedListTrait;

extern const LinkedListTrait LinkedListImpl;

#endif  // LINKEDLIST_H_
