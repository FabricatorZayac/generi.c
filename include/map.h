#ifndef MAP_H_
#define MAP_H_

#include <stdbool.h>

#include "list.h"

#define MAP(S, K, V, ...)                                           \
    bool (*contains_key)(const S *self, const K __VA_OPT__(*) key); \
    const V *(*get)(const S *self, const K __VA_OPT__(*) key);      \
    V *(*get_mut)(S * self, const K __VA_OPT__(*) key);             \
    InsertRes (*insert)(S * self, K __VA_OPT__(*) key, V __VA_OPT__(*) value);

#define PROXY_MAP(S, K, V)                              \
    bool S##_contains_key(const S *self, const K key) { \
        return self->impl.contains_key(self, &key);     \
    }                                                   \
    const V *S##_get(const S *self, const K key) {      \
        return (const V *)self->impl.get(self, &key);   \
    }                                                   \
    V *S##_get_mut(S *self, const K key) {              \
        return self->impl.get_mut(self, &key);          \
    }                                                   \
    InsertRes S##_insert(S *self, K key, V value) {     \
        return self->impl.insert(self, &key, &value);   \
    }

#define PROXY_ASSIGN_MAP(S)                                                   \
    .contains_key = S##_contains_key, .get = S##_get, .get_mut = S##_get_mut, \
    .insert = S##_insert

#endif  // MAP_H_
