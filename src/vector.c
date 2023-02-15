#include "vector.h"
#include <stddef.h>
#include <stdlib.h>

void Vec_destroy(void *self) {
    free(((Vec(void *) *)self)->body);
}

void Vec_realloc(void *self) {
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

const _GenericVecTrait Vec = {.destroy = Vec_destroy,
                              .realloc = Vec_realloc,
                              .shrink_to = Vec_shrink_to,
                              .shrink_to_fit = Vec_shrink_to_fit,
                              .reserve = Vec_reserve,
                              .truncate = Vec_truncate};
