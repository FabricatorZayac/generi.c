#ifndef VECTOR_H_
#define VECTOR_H_

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>

#include "list.h"
#include "macros.h"

#define Vec(T)                                     \
    struct ADHOC {                                 \
        T *body;                                   \
        size_t size;                               \
        size_t capacity;                           \
        const size_t element_size;                 \
        const VecTrait impl;                       \
        void (*push_back)(struct ADHOC *, T);      \
        void (*push_front)(struct ADHOC *, T);     \
        T *(*pop_front)(struct ADHOC *);           \
        T *(*pop_back)(struct ADHOC *);            \
        void (*put)(struct ADHOC *, size_t, T);    \
        T *(*get)(struct ADHOC *, size_t);         \
        void (*insert)(struct ADHOC *, size_t, T); \
        T *(*remove)(struct ADHOC *, size_t);      \
    }

/* remove and pop methods return owned values that need to be freed */
#define DefineVec(NewType, T)                                     \
    typedef Vec(T) NewType;                                       \
    void NewType##_push_back(NewType *self, T value) {            \
        return self->impl.list.push_back(self, (char *)&value);   \
    }                                                             \
    void NewType##_push_front(NewType *self, T value) {           \
        return self->impl.list.push_front(self, (char *)&value);  \
    }                                                             \
    T *NewType##_pop_back(NewType *self) {                        \
        return self->impl.list.pop_back(self);                    \
    }                                                             \
    T *NewType##_pop_front(NewType *self) {                       \
        return self->impl.list.pop_front(self);                   \
    }                                                             \
    void NewType##_put(NewType *self, size_t index, T value) {    \
        return self->impl.list.put(self, index, (char *)&value);  \
    }                                                             \
    T *NewType##_get(NewType *self, size_t index) {               \
        return self->impl.list.get(self, index);                  \
    }                                                             \
    void NewType##_insert(NewType *self, size_t index, T value) { \
        self->impl.list.insert(self, index, (char *)&value);      \
    }                                                             \
    T *NewType##_remove(NewType *self, size_t index) {            \
        return self->impl.list.remove(self, index);               \
    }                                                             \
    NewType NewType##_new() {                                     \
        return (NewType) {.size = 0,                              \
                          .capacity = 0,                          \
                          .element_size = sizeof(T),              \
                          .body = NULL,                           \
                          .impl = VecImpl,                        \
                          .put = NewType##_put,                   \
                          .get = NewType##_get,                   \
                          .push_back = NewType##_push_back,       \
                          .push_front = NewType##_push_front,     \
                          .pop_front = NewType##_pop_front,       \
                          .pop_back = NewType##_pop_back,         \
                          .insert = NewType##_insert,             \
                          .remove = NewType##_remove};            \
    }                                                             \
    NewType NewType##_with_capacity(size_t capacity) {            \
        NewType self = NewType##_new();                           \
        self.capacity = capacity;                                 \
        self.body = malloc(capacity * sizeof(T));                 \
        return self;                                              \
    }                                                             \
    NewType NewType##_from_arr(T *arr, size_t bytecap) {          \
        NewType self = NewType##_new();                           \
        self.size = bytecap / sizeof(T);                          \
        self.capacity = bytecap / sizeof(T);                      \
        self.body = memcpy(malloc(bytecap), arr, bytecap);        \
        return self;                                              \
    }                                                             \
    const struct {                                                \
        NewType (*new)();                                         \
        NewType (*with_capacity)(size_t capacity);                \
        NewType (*from_arr)(T * arr, size_t bytecap);             \
    } NewType##_ = {.new = NewType##_new,                         \
                    .with_capacity = NewType##_with_capacity,     \
                    .from_arr = NewType##_from_arr};

typedef struct {
    ListTrait list;
    void (*destroy)(void *self);
    void (*resize)(void *self);
    void (*shrink_to_fit)(void *self);
    void (*shrink_to)(void *self, size_t new_cap);
    void (*reserve)(void *self, size_t additional);
    void (*truncate)(void *self, size_t new_cap);
} VecTrait;

extern const VecTrait VecImpl;

#endif // VECTOR_H_
