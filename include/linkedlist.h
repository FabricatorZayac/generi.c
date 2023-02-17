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
        void (*push_back)(struct ADHOC *, T);          \
        T *(*pop_front)(struct ADHOC *);               \
        void (*put)(struct ADHOC *, size_t, T);        \
        T *(*get)(struct ADHOC *, size_t);             \
    }

#define DefineLinkedList(NewType, T)                                    \
    typedef LinkedList(T) NewType;                                      \
    void NewType##_push_back(NewType *self, T value) {                  \
        self->impl.list.push_back(self, (char *)&value);                \
    }                                                                   \
    T *NewType##_pop_front(NewType *self) {                             \
        return self->impl.pop_front(self);                              \
    }                                                                   \
    T *NewType##_get(NewType *self, size_t index) {                     \
        return self->impl.list.get(self, index);                        \
    }                                                                   \
    NewType _##NewType##_new() {                                        \
        return (NewType){.element_size = sizeof(T),                     \
                         .head = NULL,                                  \
                         .impl = _LinkedListImpl,                       \
                         .get = NewType##_get,                          \
                         .pop_front = NewType##_pop_front,              \
                         .push_back = NewType##_push_back};             \
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
