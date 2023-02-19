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
#define LinkedList(T)                              \
    struct ADHOC {                                 \
        LinkedNode(T) * head;                      \
        struct TOKENPASTE(ADHOC, _node) * tail;    \
        size_t size;                               \
        const size_t element_size;                 \
        const LinkedListTrait impl;                \
        void (*push_back)(struct ADHOC *, T);      \
        void (*push_front)(struct ADHOC *, T);     \
        T *(*pop_front)(struct ADHOC *);           \
        T *(*pop_back)(struct ADHOC *);            \
        void (*put)(struct ADHOC *, size_t, T);    \
        T *(*get)(struct ADHOC *, size_t);         \
        void (*insert)(struct ADHOC *, size_t, T); \
        T *(*remove)(struct ADHOC *, size_t);      \
    }

#define DefineLinkedList(NewType, T)                                           \
    typedef LinkedList(T) NewType;                                             \
    void NewType##_push_back(NewType *self, T value) {                         \
        self->impl.push_back(self, (char *)&value);                            \
    }                                                                          \
    void NewType##_push_front(NewType *self, T value) {                        \
        self->impl.push_front(self, (char *)&value);                           \
    }                                                                          \
    T *NewType##_pop_front(NewType *self) {                                    \
        return self->impl.pop_front(self);                                     \
    }                                                                          \
    T *NewType##_pop_back(NewType *self) { return self->impl.pop_back(self); } \
    T *NewType##_get(NewType *self, size_t index) {                            \
        return self->impl.get(self, index);                                    \
    }                                                                          \
    void NewType##_insert(NewType *self, size_t index, T value) {              \
        return self->impl.insert(self, index, (char *)&value);                 \
    }                                                                          \
    T *NewType##_remove(NewType *self, size_t index) {                         \
        return self->impl.remove(self, index);                                 \
    }                                                                          \
    NewType NewType##_new() {                                                  \
        return (NewType) {.element_size = sizeof(T),                           \
                          .head = NULL,                                        \
                          .size = 0,                                           \
                          .impl = LinkedListImpl,                              \
                          .get = NewType##_get,                                \
                          .insert = NewType##_insert,                          \
                          .remove = NewType##_remove,                          \
                          .pop_front = NewType##_pop_front,                    \
                          .pop_back = NewType##_pop_back,                      \
                          .push_back = NewType##_push_back,                    \
                          .push_front = NewType##_push_front};                 \
    }                                                                          \
    const struct {                                                             \
        NewType (*new)();                                                      \
    } NewType##_ = {.new = NewType##_new};

typedef struct {
    DERIVE_LIST
    void (*destroy)(void *self);
} LinkedListTrait;

extern const LinkedListTrait LinkedListImpl;

#endif  // LINKEDLIST_H_
