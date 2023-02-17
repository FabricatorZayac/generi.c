#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "list.h"
#include "macros.h"

#define LinkedNode(T)                                  \
    struct TOKENPASTE(ADHOC, _node) {                  \
        T body;                                        \
        struct TOKENPASTE(ADHOC, _node) *next;         \
    }

#define LinkedList(T)                                  \
    struct ADHOC {                                     \
        LinkedNode(T) *head;                           \
        const size_t element_size;                     \
        const LinkedListTrait impl;                    \
        void (*push)(struct ADHOC *, T);               \
        T *(*pop_front)(struct ADHOC *);               \
        void (*put)(struct ADHOC *, size_t, T);        \
        T *(*get)(struct ADHOC *, size_t);             \
    }

#define DefineLinkedList(NewType, T)                                    \
    typedef LinkedList(T) NewType;                                      \
    void _##NewType##_push(NewType *self, T value) {                    \
        self->impl.list.push(self, (uint8_t *)&value);                  \
    }                                                                   \
    T *_##NewType##_pop_front(NewType *self) {                          \
        return self->impl.pop_front(self);                              \
    }                                                                   \
    T *_##NewType##_get(NewType *self, size_t index) {                  \
        return self->impl.list.get(self, index);                        \
    }                                                                   \
    NewType _##NewType##_new() {                                        \
        return (NewType){.element_size = sizeof(T),                     \
                         .head = NULL,                                  \
                         .impl = _LinkedListImpl,                       \
                         .get = _##NewType##_get,                       \
                         .pop_front = _##NewType##_pop_front,           \
                         .push = _##NewType##_push};                    \
    }                                                                   \
    const struct {                                                      \
        NewType (*new)();                                               \
    } NewType##_ = {.new = _##NewType##_new};

typedef struct {
    ListTrait list;
    void *(*pop_front)(void *self);
    void (*destroy)(void *self);
    void (*append)(void *self, void *);
} LinkedListTrait;

extern const LinkedListTrait _LinkedListImpl;

#endif // LINKEDLIST_H_
