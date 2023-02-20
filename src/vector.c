#include "vector.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

void Vec_destroy(void *self) {
    free(((Vec(void *) *)self)->body);
}

void Vec_resize(void *self) {
    Vec(void *) *vec = self;
    if (vec->size == 0) {
        vec->body = malloc(vec->element_size);
        vec->capacity = 1;
    } else {
        vec->body = realloc(vec->body, 2 * vec->element_size * vec->capacity);
        vec->capacity *= 2;
    }
}

void Vec_reserve(void *self, size_t additional) {
    Vec(void *) *vec = self;
    if (vec->capacity < vec->size + additional) {
        vec->body =
            realloc(vec->body, (vec->size + additional) * vec->element_size);
        vec->capacity = vec->size + additional;
    }
}

void Vec_shrink_to_fit(void *self) {
    Vec(void *) *vec = self;
    if (vec->capacity > vec->size) {
        vec->body = realloc(vec->body, vec->element_size * vec->size);
        vec->capacity = vec->size;
    }
}

void Vec_shrink_to(void *self, size_t min_cap) {
    Vec(void *) *vec = self;
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
    Vec(void *) *vec = self;
    if (new_size <= vec->size) {
        vec->body = realloc(vec->body, new_size * vec->element_size);
        vec->capacity = new_size;
        vec->size = new_size;
    }
}

void *Vec_get(void *self, size_t index) {
    Vec(char) *vec = self;
    if (index < vec->capacity) {
        return &vec->body[vec->element_size * index];
    } else {
        return NULL;
    }
}

void *Vec_pop_back(void *self) {
    Vec(char) *vec = self;
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
    Vec(char) *vec = self;
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
    Vec(char) *vec = self;
    if (vec->size >= vec->capacity) {
        vec->impl.resize(vec);
    }
    memcpy(vec->body + (vec->size * vec->element_size),
           value_address,
           vec->element_size);
    vec->size++;
}

void Vec_push_front(void *self, void *value_address) {
    Vec(char) *vec = self;
    if (vec->size >= vec->capacity) {
        Vec_resize(vec);
    }
    memmove(vec->body + vec->element_size,
            vec->body,
            vec->size * vec->element_size);
    memcpy(vec->body, value_address, vec->element_size);
    vec->size++;
}

void Vec_put(void *self, size_t index, void *value_address) {
    errno = 0;
    Vec(char) *vec = self;

    if (index < vec->capacity) {
        memcpy(vec->body + (index * vec->element_size),
               value_address,
               vec->element_size);
        if (vec->size < index) vec->size = index;
    } else {
        errno = ERANGE;
    }
}

void Vec_append(void *self, void *other) {
    Vec(char) *vec = self;
    Vec(char) *other_vec = other;

    if (vec->capacity < vec->size + other_vec->size) {
        vec->impl.reserve(vec, other_vec->size);
    }
    memcpy(vec->body + vec->size * vec->element_size,
           other_vec->body,
           other_vec->size * vec->element_size);
    vec->size += other_vec->size;
    other_vec->impl.truncate(other_vec, 0);
}

void Vec_insert(void *self, size_t index, void *value_address) {
    Vec(char) *vec = self;
    errno = 0;
    if (index == 0) {
        Vec_push_front(vec, value_address);
    } else if (index == vec->size - 1) {
        Vec_push_back(vec, value_address);
    } else {
        void *at = vec->get(vec, index);
        if (at == NULL) {
            errno = ERANGE;
            return;
        }
        memmove(at + vec->element_size,
                at,
                vec->element_size * (vec->size - index));
        memcpy(at, value_address, vec->element_size);
    }
}

void *Vec_remove(void *self, size_t index) {
    Vec(char) *vec = self;
    if (index == 0) {
        return vec->pop_front(vec);
    } else if (index == vec->size - 1) {
        return vec->pop_back(vec);
    } else {
        void *at = vec->get(vec, index);
        if (at == NULL) return NULL;
        void *result = memcpy(malloc(vec->element_size), at, vec->element_size);
        memmove(at,
                at + vec->element_size,
                vec->element_size * (vec->size - index));
        vec->size--;
        return result;
    }
}

const VecTrait VecImpl = {.get = Vec_get,
                          .pop_back = Vec_pop_back,
                          .pop_front = Vec_pop_front,
                          .push_back = Vec_push_back,
                          .push_front = Vec_push_front,
                          .put = Vec_put,
                          .append = Vec_append,
                          .insert = Vec_insert,
                          .remove = Vec_remove,
                          .destroy = Vec_destroy,
                          .resize = Vec_resize,
                          .shrink_to = Vec_shrink_to,
                          .shrink_to_fit = Vec_shrink_to_fit,
                          .reserve = Vec_reserve,
                          .truncate = Vec_truncate};
