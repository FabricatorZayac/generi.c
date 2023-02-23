#ifndef MAP_H_
#define MAP_H_

#include <stdbool.h>

#include "list.h"

#define MAP(S, K, V, ...)                                \
    bool (*contains_key)(S * self, K __VA_OPT__(*) key); \
    V *(*get)(S * self, K __VA_OPT__(*) key);            \
    InsertRes (*insert)(S * self, K __VA_OPT__(*) key, V __VA_OPT__(*) value);

#define PROXY_MAP(S, K, V)                                            \
    bool S##_contains_key(S *self, K key) {                           \
        return self->impl.contains_key(self, &key);                   \
    }                                                                 \
    V *S##_get(S *self, K key) { return self->impl.get(self, &key); } \
    InsertRes S##_insert(S *self, K key, V value) {                   \
        return self->impl.insert(self, &key, &value);                 \
    }

#define PROXY_ASSIGN_MAP(S) \
    .contains_key = S##_contains_key, .get = S##_get, .insert = S##_insert,

#endif  // MAP_H_
