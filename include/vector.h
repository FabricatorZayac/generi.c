#ifndef VECTOR_H_
#define VECTOR_H_

#include "list.h"
#include "macros.h"
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>

#define Vec(T)                                   \
    struct ADHOC {                               \
        T *body;                                 \
        size_t size;                             \
        size_t capacity;                         \
        const size_t element_size;               \
        const VecTrait impl;                     \
        void (*push_back)(struct ADHOC *, T);    \
        T *(*pop_back)(struct ADHOC *);          \
        void (*put)(struct ADHOC *, size_t, T);  \
        T *(*get)(struct ADHOC *, size_t);       \
    }

#define DefineVec(NewType, T)                                           \
    typedef Vec(T) NewType;                                             \
    void _##NewType##_push_back(NewType *self, T value) {               \
        return self->impl.list.push_back(self, (uint8_t *)&value);      \
    }                                                                   \
    T *_##NewType##_pop_back(NewType *self) {                           \
        return self->impl.list.pop_back(self);                          \
    }                                                                   \
    void _##NewType##_put(NewType *self, size_t index, T value) {       \
        return self->impl.list.put(self, index, (uint8_t *)&value);     \
    }                                                                   \
    T *_##NewType##_get(NewType *self, size_t index) {                  \
        return self->impl.list.get(self, index);                        \
    }                                                                   \
    NewType _##NewType##_new() {                                        \
        return (NewType){.size = 0,                                     \
                         .capacity = 0,                                 \
                         .element_size = sizeof(T),                     \
                         .body = NULL,                                  \
                         .impl = _VecImpl,                              \
                         .put = _##NewType##_put,                       \
                         .get = _##NewType##_get,                       \
                         .push_back = _##NewType##_push_back,           \
                         .pop_back = _##NewType##_pop_back};            \
    }                                                                   \
    NewType _##NewType##_with_capacity(size_t capacity) {               \
        return (NewType){.size = 0,                                     \
                         .capacity = capacity,                          \
                         .element_size = sizeof(T),                     \
                         .body = malloc(capacity * sizeof(T)),          \
                         .impl = _VecImpl,                              \
                         .put = _##NewType##_put,                       \
                         .get = _##NewType##_get,                       \
                         .push_back = _##NewType##_push_back,           \
                         .pop_back = _##NewType##_pop_back};            \
    }                                                                   \
    NewType _##NewType##_from_arr(T *arr, size_t bytecap) {             \
        return (NewType){.size = bytecap / sizeof(T),                   \
                         .capacity = bytecap / sizeof(T),               \
                         .element_size = sizeof(T),                     \
                         .body = memcpy(malloc(bytecap), arr, bytecap), \
                         .impl = _VecImpl,                              \
                         .put = _##NewType##_put,                       \
                         .get = _##NewType##_get,                       \
                         .push_back = _##NewType##_push_back,           \
                         .pop_back = _##NewType##_pop_back};            \
    }                                                                   \
    const struct {                                                      \
        NewType (*new)();                                               \
        NewType (*with_capacity)(size_t capacity);                      \
        NewType (*from_arr)(T *arr, size_t bytecap);                    \
    } NewType##_ = {                                                    \
        .new = _##NewType##_new,                                        \
        .with_capacity = _##NewType##_with_capacity,                    \
        .from_arr = _##NewType##_from_arr};                             \

typedef struct {
    ListTrait list;
    void (*destroy)(void *self);
    void (*realloc)(void *self);
    void (*shrink_to_fit)(void *self);
    void (*shrink_to)(void *self, size_t new_cap);
    void (*reserve)(void *self, size_t additional);
    void (*truncate)(void *self, size_t new_cap);
} VecTrait;

extern const VecTrait _VecImpl;

#endif // VECTOR_H_
