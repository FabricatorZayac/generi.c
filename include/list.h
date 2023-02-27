#ifndef LIST_H_
#define LIST_H_

#include <stddef.h>

#include "error.h"
#include "macros.h"
#include "result.h"

typedef Result(struct {}, Error) InsertRes;

#define LIST(S, T, ...)                                                 \
    const T *(*get)(const S *self, const size_t index);                 \
    T *(*get_mut)(S * self, size_t index);                              \
    void (*push_back)(S * self, T __VA_OPT__(*) value);                 \
    void (*push_front)(S * self, T __VA_OPT__(*) value);                \
    T *(*pop_back)(S * self);                                           \
    T *(*pop_front)(S * self);                                          \
    void (*append)(S * self, S * other);                                \
    InsertRes (*insert)(S * self, size_t index, T __VA_OPT__(*) value); \
    T *(*remove)(S * self, size_t index);

#define PROXY_LIST(S, T)                                                 \
    static inline const T *S##_get(const S *self, const size_t index) {  \
        return self->impl.get(self, index);                              \
    }                                                                    \
    static inline T *S##_get_mut(S *self, size_t index) {                \
        return self->impl.get_mut(self, index);                          \
    }                                                                    \
    static inline void S##_push_back(S *self, T value) {                 \
        self->impl.push_back(self, &value);                              \
    }                                                                    \
    static inline void S##_push_front(S *self, T value) {                \
        self->impl.push_front(self, &value);                             \
    }                                                                    \
    static inline T *S##_pop_back(S *self) {                             \
        return self->impl.pop_back(self);                                \
    }                                                                    \
    static inline T *S##_pop_front(S *self) {                            \
        return self->impl.pop_front(self);                               \
    }                                                                    \
    static inline void S##_append(S *self, S *other) {                   \
        self->impl.append(self, other);                                  \
    }                                                                    \
    static inline InsertRes S##_insert(S *self, size_t index, T value) { \
        return self->impl.insert(self, index, &value);                   \
    }                                                                    \
    static inline T *S##_remove(S *self, size_t index) {                 \
        return self->impl.remove(self, index);                           \
    }

#define PROXY_ASSIGN_LIST(S)                                                \
    .get = S##_get, .get_mut = S##_get_mut, .push_back = S##_push_back,     \
    .push_front = S##_push_front, .pop_back = S##_pop_back,                 \
    .pop_front = S##_pop_front, .append = S##_append, .insert = S##_insert, \
    .remove = S##_remove

#endif  // LIST_H_
