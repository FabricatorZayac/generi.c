#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"

void ll_destroy(void *self) {
    LinkedList(void *)*list = self;
    LinkedNode(void *) *current = (void *)list->head;
    while (current != NULL) {
        LinkedNode(void *) *buf = (void *)current;
        current = current->next;
        free(buf);
    }
    free(current);
}

void ll_append(void *self, void *other) {
    LinkedList(void *) *list = self;
    LinkedList(void *) *other_list = other;
    LinkedNode(void *) *tail = (void *)list->head;
    while (tail != NULL) {
        tail->next = (void *)list->head;
    }
    tail = (void *)other_list->head;
}

void ll_push(void *self, uint8_t *value_bytes) {
    LinkedList(uint8_t *) *list = self;
    typedef LinkedNode(uint8_t *) bytenode;
    bytenode *new_node = malloc(sizeof(bytenode));
    memmove(&new_node->body, value_bytes, list->element_size);

    if (list->head == NULL) {
        new_node->next = NULL;
        list->head = (void *)new_node;
        return;
    } else {
        LinkedNode(uint8_t *) *i = (void *)list->head;
        for (; i->next != NULL; i = i->next) {}
        i->next = (void *)new_node;
    }
}

void *ll_pop_front(void *self) {
    LinkedList(void *) *list = self;
    if (list->head != NULL) {
        void *result = memmove(malloc(list->element_size),
                               &list->head->body,
                               list->element_size);
        void *buf = list->head;
        list->head = list->head->next;
        free(buf);
        return result;
    } else {
        return NULL;
    }
}

void *ll_get(void *self, size_t index) {
    LinkedList(void *) *list = self;
    LinkedNode(void *) *current = (void *)list->head;
    for (size_t i = 0; i < index; i++) {
        if (current->next == NULL) return NULL;
        current = current->next;
    }
    return &current->body;
}

const LinkedListTrait _LinkedListImpl = {
    .list = {.push = ll_push, .get = ll_get},
    .pop_front = ll_pop_front,
    .destroy = ll_destroy,
    .append = ll_append};
