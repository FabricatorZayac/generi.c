#include "hashmap.h"

#include <string.h>

#include "list.h"
#include "vector.h"

DefineHashMap(Voidmap, void *, void *);

void hm_destroy(void *self) {
    Voidmap *map = self;
    map->body.impl.destroy(&map->body);
}

size_t _hash(void *self, void *key) {
    Voidmap *map = self;
    return *(size_t *)key % map->body.capacity;
}

InsertRes hm_insert(void *self, void *key, void *value) {
    Voidmap *map = self;
    Voidmap_entry *at = &map->body.body[_hash(self, key)];
    if (at->key == 0) {
        // FIXME: segfaults, I am too tired
        memcpy(&at->key, key, map->key_size);
        memcpy(&at->value, value, map->value_size);
    } else {
        // TODO: add other situations
        return Err(InsertRes, "Key not zero");
    }
    return Ok(InsertRes, {});
}

const HashMapTrait HashMapImpl = {.destroy = hm_destroy, .insert = hm_insert};
