#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "list.h"
#include "macros.h"

#define LinkedNode(T)                                  \
    struct TOKENPASTE(ADHOC, _node) {                  \
        T body;                                        \
        struct TOKENPASTE(ADHOC, _node) * next, *prev; \
    }

/* remove and pop methods return owned values that need to be freed */
#define LinkedList(T)                           \
    struct ADHOC {                              \
        LinkedNode(T) * head;                   \
        struct TOKENPASTE(ADHOC, _node) * tail; \
        size_t size;                            \
        const size_t element_size;              \
        const LinkedListTrait impl;             \
        DERIVE(LIST, T)                         \
    }

#define DefineLinkedList(NewType, T)                    \
    typedef LinkedList(T) NewType;                      \
    PROXY(LIST, NewType, T)                             \
    NewType NewType##_new() {                           \
        return (NewType) {.element_size = sizeof(T),    \
                          .head = NULL,                 \
                          .size = 0,                    \
                          .impl = LinkedListImpl,       \
                          PROXY_ASSIGN(LIST, NewType)}; \
    }                                                   \
    const struct {                                      \
        NewType (*new)();                               \
    } NewType##_ = {.new = NewType##_new};

typedef struct {
    DERIVE_TRAIT(LIST)
    void (*destroy)(void *self);
} LinkedListTrait;

extern const LinkedListTrait LinkedListImpl;

#endif  // LINKEDLIST_H_
