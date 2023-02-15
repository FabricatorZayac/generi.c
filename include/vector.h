#ifndef VECTOR_H_
#define VECTOR_H_

#include "macros.h"
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

#define VecBuilder(T)                \
    struct {                         \
        T (*new)();                  \
        T (*with_capacity)(size_t);  \
    }

#define Vec(T)                                   \
    struct ADHOC {                               \
        T *body;                                 \
        size_t size;                             \
        size_t capacity;                         \
        size_t element_size;                     \
        void (*put)(struct ADHOC *, size_t, T);  \
        T *(*get)(struct ADHOC *, size_t);       \
        void (*push)(struct ADHOC *, T);         \
        T *(*pop)(struct ADHOC *);               \
    }

#define DefineVec(NewType, T)                                           \
    typedef Vec(T) NewType;                                             \
    T *_NewType##_get(NewType *self, size_t index) {                    \
        if (index < self->capacity) {                                   \
            return &self->body[index];                                  \
        } else {                                                        \
            return NULL;                                                \
        }                                                               \
    }                                                                   \
    void _NewType##_put(NewType *self, size_t index, T value) {         \
        errno = 0;                                                      \
        if (index < self->capacity) {                                   \
            self->body[index] = value;                                  \
            if (self->size < index) self->size = index;                 \
        } else {                                                        \
            errno = ERANGE;                                             \
        }                                                               \
    }                                                                   \
    void _NewType##_push(NewType *self, T value) {                      \
        if (self->size < self->capacity) {                              \
            self->body[self->size++] = value;                           \
        } else {                                                        \
            Vec.realloc(self);                                          \
            self->body[self->size++] = value;                           \
        }                                                               \
    }                                                                   \
    T *_NewType##_pop(NewType *self) {                                  \
        if (self->size != 0){                                           \
            return &self->body[--self->size];                           \
        } else {                                                        \
            return NULL;                                                \
        }                                                               \
    }                                                                   \
    NewType _NewType##_new() {                                          \
        return (NewType){.size = 0,                                     \
                         .capacity = 0,                                 \
                         .element_size = sizeof(T),                     \
                         .body = NULL,                                  \
                         .put = _NewType##_put,                         \
                         .get = _NewType##_get,                         \
                         .push = _NewType##_push,                       \
                         .pop = _NewType##_pop};                        \
    }                                                                   \
    NewType _NewType##_with_capacity(size_t initial_capacity) {         \
        return (NewType){.size = 0,                                     \
                         .capacity = initial_capacity,                  \
                         .element_size = sizeof(T),                     \
                         .body = malloc(initial_capacity * sizeof(T)),  \
                         .put = _NewType##_put,                         \
                         .get = _NewType##_get,                         \
                         .push = _NewType##_push,                       \
                         .pop = _NewType##_pop};                        \
    }                                                                   \
    const VecBuilder(NewType) NewType##_ = {                            \
        .new = _NewType##_new,                                          \
        .with_capacity = _NewType##_with_capacity};                     \


typedef struct {
    void (*destroy)(void *);
    void (*realloc)(void *);
    void (*shrink_to_fit)(void *);
    void (*shrink_to)(void *, size_t);
    void (*reserve)(void *, size_t);
    void (*truncate)(void *, size_t);
} _GenericVecTrait;

extern const _GenericVecTrait Vec;

#endif // VECTOR_H_
