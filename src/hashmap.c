#include "hashmap.h"

#include <string.h>

#include "list.h"
#include "macros.h"
#include "vector.h"

DefineHashMap(Voidmap, void *, void *);

// TODO: test everything
void hm_destroy(void *self) {
    Voidmap *map = self;
    map->body.impl.destroy(&map->body);
}

const size_t get_index(const void *self, const void *key) {
    const Voidmap *map = self;
    return *(size_t *)key % map->body.capacity;
}

bool hm_contains_key(const void *self, const void *key) {
    const Voidmap *map = self;
    if (map->body.get(&map->body, get_index(map, key)) == NULL) {
        return false;
    } else {
        return true;
    }
}

const void *hm_get(const void *self, const void *key) {
    const Voidmap *map = self;
    if (map->contains_key(map, key)) {
        return map->body.get(&map->body, get_index(map, key));
    } else {
        return NULL;
    }
}

void *hm_get_mut(void *self, const void *key) {
    Voidmap *map = self;
    if (map->contains_key(map, key)) {
        return map->body.get_mut(&map->body, get_index(map, key));
    } else {
        return NULL;
    }
}

InsertRes hm_insert(void *self, void *key, void *value) {
    Voidmap *map = self;
    if (!hm_contains_key(map, key)) {
        Voidmap_entry *bucket = hm_get_mut(map, key);
        memcpy(bucket->key, key, map->key_size);
        memcpy(bucket->value, value, map->value_size);
    } else {
        // TODO: add collision resolution
        return Err(InsertRes, Error("Hash collision"));
    }
    return Ok(InsertRes, {});
}

const HashMapTrait HashMapImpl = {PROXY_ASSIGN(MAP, hm), .destroy = hm_destroy};
