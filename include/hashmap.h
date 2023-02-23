#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "macros.h"
#include "map.h"

#define HashMap(K, V, B)                                     \
    struct TOKENPASTE(hashmap, ADHOC) {                      \
        B body;                                              \
        const size_t key_size;                               \
        const size_t value_size;                             \
        const HashMapTrait impl;                             \
        DERIVE(MAP, struct TOKENPASTE(hashmap, ADHOC), K, V) \
    }

#define DefineHashMap(NewType, K, V)                            \
    typedef struct {                                            \
        K key;                                                  \
        V value;                                                \
    } NewType##_entry;                                          \
    DefineVec(NewType##_body, NewType##_entry);                 \
    typedef HashMap(K, V, NewType##_body) NewType;              \
    PROXY(MAP, NewType, K, V)                                   \
    NewType NewType##_new() {                                   \
        NewType##_body body = NewType##_body_with_capacity(16); \
        body.fill(&body, (NewType##_entry) {});                 \
        return (NewType) {.key_size = sizeof(K),                \
                          .value_size = sizeof(V),              \
                          .body = body,                         \
                          .impl = HashMapImpl,                  \
                          PROXY_ASSIGN(MAP, NewType)};          \
    }

typedef struct {
    DERIVE_TRAIT(MAP)
    void (*destroy)(void *self);
} HashMapTrait;

extern const HashMapTrait HashMapImpl;

#endif  // HASHMAP_H_
