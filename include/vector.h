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
        const size_t element_size;               \
        const _GenericVecTrait impl;             \
        void (*put)(struct ADHOC *, size_t, T);  \
        T *(*get)(struct ADHOC *, size_t);       \
        void (*push)(struct ADHOC *, T);         \
        T *(*pop)(struct ADHOC *);               \
    }

#define DefineVec(NewType, T)                                           \
    typedef Vec(T) NewType;                                             \
    T *_##NewType##_get(NewType *self, size_t index) {                  \
        if (index < self->capacity) {                                   \
            return &self->body[index];                                  \
        } else {                                                        \
            return NULL;                                                \
        }                                                               \
    }                                                                   \
    void _##NewType##_put(NewType *self, size_t index, T value) {       \
        errno = 0;                                                      \
        if (index < self->capacity) {                                   \
            self->body[index] = value;                                  \
            if (self->size < index) self->size = index;                 \
        } else {                                                        \
            errno = ERANGE;                                             \
        }                                                               \
    }                                                                   \
    void _##NewType##_push(NewType *self, T value) {                    \
        if (self->size < self->capacity) {                              \
            self->body[self->size++] = value;                           \
        } else {                                                        \
            Vec.realloc(self);                                          \
            self->body[self->size++] = value;                           \
        }                                                               \
    }                                                                   \
    T *_##NewType##_pop(NewType *self) {                                \
        if (self->size != 0){                                           \
            return &self->body[--self->size];                           \
        } else {                                                        \
            return NULL;                                                \
        }                                                               \
    }                                                                   \
    NewType _##NewType##_new() {                                        \
        return (NewType){.size = 0,                                     \
                         .capacity = 0,                                 \
                         .element_size = sizeof(T),                     \
                         .body = NULL,                                  \
                         .impl = Vec,                                   \
                         .put = _##NewType##_put,                       \
                         .get = _##NewType##_get,                       \
                         .push = _##NewType##_push,                     \
                         .pop = _##NewType##_pop};                      \
    }                                                                   \
    NewType _##NewType##_with_capacity(size_t initial_capacity) {       \
        return (NewType){.size = 0,                                     \
                         .capacity = initial_capacity,                  \
                         .element_size = sizeof(T),                     \
                         .body = malloc(initial_capacity * sizeof(T)),  \
                         .impl = Vec,                                   \
                         .put = _##NewType##_put,                       \
                         .get = _##NewType##_get,                       \
                         .push = _##NewType##_push,                     \
                         .pop = _##NewType##_pop};                      \
    }                                                                   \
    const VecBuilder(NewType) NewType##_ = {                            \
        .new = _##NewType##_new,                                        \
        .with_capacity = _##NewType##_with_capacity};                   \


// First arg is pointer to a Vec value
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
