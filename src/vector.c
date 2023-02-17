#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "vector.h"
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

void Vec_reserve(void *self, size_t additional) {
    Vec(void *) *vec = self;
    if (vec->capacity < vec->size + additional) {
        vec->body = realloc(vec->body, (vec->size + additional) * vec->element_size);
        vec->capacity = vec->size + additional;
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
    if (vec->size != 0){
        void *result = memcpy(malloc(vec->element_size),
                              &vec->body[--vec->size * vec->element_size],
                              vec->element_size);
        return result;
    } else {
        return NULL;
    }
}

void Vec_push_back(void *self, char *value_bytes) {
    Vec(char) *vec = self;
    if (vec->size >= vec->capacity) {
        vec->impl.realloc(vec);
    }
    memcpy(vec->body + (vec->size * vec->element_size), value_bytes, vec->element_size);
    vec->size++;
}

void Vec_put(void *self, size_t index, char *value_bytes) {
    errno = 0;
    Vec(char) *vec = self;

    if (index < vec->capacity) {
        memcpy(vec->body + (index * vec->element_size), value_bytes, vec->element_size);
        if (vec->size < index) vec->size = index;
    } else {
        errno = ERANGE;
    }
}

const VecTrait VecImpl = {.list = {.get = Vec_get,
                                   .pop_back = Vec_pop_back,
                                   .push_back = Vec_push_back,
                                   .put = Vec_put},
                          .destroy = Vec_destroy,
                          .realloc = Vec_resize,
                          .shrink_to = Vec_shrink_to,
                          .shrink_to_fit = Vec_shrink_to_fit,
                          .reserve = Vec_reserve,
                          .truncate = Vec_truncate};
