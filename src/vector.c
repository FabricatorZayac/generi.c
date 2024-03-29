#include "vector.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

DefineVec(ByteVec, char);

void Vec_destroy(void *self) {
    free(((ByteVec *)self)->body);
}

void Vec_resize(void *self) {
    ByteVec *vec = self;
    if (vec->size == 0) {
        vec->body = malloc(vec->element_size);
        vec->capacity = 1;
    } else {
        vec->body = realloc(vec->body, 2 * vec->element_size * vec->capacity);
        vec->capacity *= 2;
    }
}

void Vec_reserve(void *self, size_t additional) {
    ByteVec *vec = self;
    if (vec->capacity < vec->size + additional) {
        vec->body =
            realloc(vec->body, (vec->size + additional) * vec->element_size);
        vec->capacity = vec->size + additional;
    }
}

void Vec_shrink_to_fit(void *self) {
    ByteVec *vec = self;
    if (vec->capacity > vec->size) {
        vec->body = realloc(vec->body, vec->element_size * vec->size);
        vec->capacity = vec->size;
    }
}

void Vec_shrink_to(void *self, size_t min_cap) {
    ByteVec *vec = self;
    if (vec->capacity > vec->size && vec->capacity > min_cap) {
        size_t new_cap;
        if (min_cap > vec->size) {
            new_cap = min_cap;
        } else {
            new_cap = vec->size;
        }
        vec->body = realloc(vec->body, new_cap * vec->element_size);
        vec->capacity = new_cap;
    }
}

void Vec_truncate(void *self, size_t new_size) {
    ByteVec *vec = self;
    if (new_size <= vec->size) {
        vec->body = realloc(vec->body, new_size * vec->element_size);
        vec->capacity = new_size;
        vec->size = new_size;
    }
}

void *Vec_get_mut(void *self, size_t index) {
    ByteVec *vec = self;
    if (index < vec->capacity) {
        return &vec->body[vec->element_size * index];
    } else {
        return NULL;
    }
}

const void *Vec_get(const void *self, const size_t index) {
    const ByteVec *vec = self;
    if (index < vec->capacity) {
        return &vec->body[vec->element_size * index];
    } else {
        return NULL;
    }
}

void *Vec_pop_back(void *self) {
    ByteVec *vec = self;
    if (vec->size != 0) {
        vec->size--;
        void *result = memcpy(malloc(vec->element_size),
                              &vec->body[vec->size * vec->element_size],
                              vec->element_size);
        return result;
    } else {
        return NULL;
    }
}

void *Vec_pop_front(void *self) {
    ByteVec *vec = self;
    if (vec->size != 0) {
        void *result =
            memcpy(malloc(vec->element_size), &vec->body[0], vec->element_size);
        memmove(vec->body,
                vec->body + vec->element_size,
                vec->element_size * vec->size - 1);
        vec->size--;
        return result;
    } else {
        return NULL;
    }
}

void Vec_push_back(void *self, void *value_address) {
    ByteVec *vec = self;
    if (vec->size >= vec->capacity) {
        vec->impl.resize(vec);
    }
    memcpy(vec->body + (vec->size * vec->element_size),
           value_address,
           vec->element_size);
    vec->size++;
}

void Vec_push_front(void *self, void *value_address) {
    ByteVec *vec = self;
    if (vec->size >= vec->capacity) {
        Vec_resize(vec);
    }
    memmove(vec->body + vec->element_size,
            vec->body,
            vec->size * vec->element_size);
    memcpy(vec->body, value_address, vec->element_size);
    vec->size++;
}

void Vec_append(void *self, void *other) {
    ByteVec *vec = self;
    ByteVec *other_vec = other;

    if (vec->capacity < vec->size + other_vec->size) {
        vec->impl.reserve(vec, other_vec->size);
    }
    memcpy(vec->body + vec->size * vec->element_size,
           other_vec->body,
           other_vec->size * vec->element_size);
    vec->size += other_vec->size;
    other_vec->impl.truncate(other_vec, 0);
}

InsertRes Vec_insert(void *self, size_t index, void *value_address) {
    ByteVec *vec = self;
    if (index == 0) {
        Vec_push_front(vec, value_address);
    } else if (index == vec->size - 1) {
        Vec_push_back(vec, value_address);
    } else {
        void *at = vec->get_mut(vec, index);
        if (at == NULL) {
            return Err(InsertRes, Error("Index out of range"));
        }
        memmove(at + vec->element_size,
                at,
                vec->element_size * (vec->size - index));
        memcpy(at, value_address, vec->element_size);
    }
    return Ok(InsertRes, {});
}

void *Vec_remove(void *self, size_t index) {
    ByteVec *vec = self;
    if (index == 0) {
        return vec->pop_front(vec);
    } else if (index == vec->size - 1) {
        return vec->pop_back(vec);
    } else {
        void *at = vec->get_mut(vec, index);
        if (at == NULL) return NULL;
        void *result = memcpy(malloc(vec->element_size), at, vec->element_size);
        memmove(at,
                at + vec->element_size,
                vec->element_size * (vec->size - index));
        vec->size--;
        return result;
    }
}

// TODO: test
void Vec_fill(void *self, void *value_address) {
    ByteVec *vec = self;
    for (size_t i = 0; i < vec->capacity; i++) {
        memcpy(vec->body + (vec->element_size * i),
               value_address,
               vec->element_size);
    }
    vec->size = vec->capacity;
}

const VecTrait VecImpl = {PROXY_ASSIGN(LIST, Vec),
                          .destroy = Vec_destroy,
                          .resize = Vec_resize,
                          .shrink_to = Vec_shrink_to,
                          .shrink_to_fit = Vec_shrink_to_fit,
                          .reserve = Vec_reserve,
                          .truncate = Vec_truncate,
                          .fill = Vec_fill};
