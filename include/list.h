#ifndef LIST_H_
#define LIST_H_

#include <stddef.h>

#include "macros.h"

#define LIST(T, S, ...)                                                        \
    T *(*get)(S * self, size_t index);                                         \
    void (*put)(S * self,                                                      \
                size_t index,                                                  \
                T __VA_OPT__(*) value##__VA_OPT__(_address));                  \
    void (*push_back)(S * self, T __VA_OPT__(*) value##__VA_OPT__(_address));  \
    void (*push_front)(S * self, T __VA_OPT__(*) value##__VA_OPT__(_address)); \
    T *(*pop_back)(S * self);                                                  \
    T *(*pop_front)(S * self);                                                 \
    void (*append)(S * self, S * other);                                       \
    void (*insert)(S * self,                                                   \
                   size_t index,                                               \
                   T __VA_OPT__(*) value##__VA_OPT__(_address));               \
    T *(*remove)(S * self, size_t index);

#define PROXY_LIST(S, T)                                                      \
    T *S##_get(S *self, size_t index) { return self->impl.get(self, index); } \
    void S##_put(S *self, size_t index, T value) {                            \
        return self->impl.put(self, index, &value);                           \
    }                                                                         \
    void S##_push_back(S *self, T value) {                                    \
        self->impl.push_back(self, &value);                                   \
    }                                                                         \
    void S##_push_front(S *self, T value) {                                   \
        self->impl.push_front(self, &value);                                  \
    }                                                                         \
    T *S##_pop_back(S *self) { return self->impl.pop_back(self); }            \
    T *S##_pop_front(S *self) { return self->impl.pop_front(self); }          \
    void S##_append(S *self, S *other) { self->impl.append(self, &other); }   \
    void S##_insert(S *self, size_t index, T value) {                         \
        self->impl.insert(self, index, &value);                               \
    }                                                                         \
    T *S##_remove(S *self, size_t index) {                                    \
        return self->impl.remove(self, index);                                \
    }

#define PROXY_ASSIGN_LIST(S)                                                \
    .get = S##_get, .put = S##_put, .push_back = S##_push_back,             \
    .push_front = S##_push_front, .pop_back = S##_pop_back,                 \
    .pop_front = S##_pop_front, .append = S##_append, .insert = S##_insert, \
    .remove = S##_remove,

#endif  // LIST_H_
