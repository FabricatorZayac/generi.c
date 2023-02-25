#include "linkedlist.h"

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ll_destroy(void *self) {
    LinkedList(void *) *list = self;
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
    list->tail->next = (void *)other_list->head;
    other_list->head->prev = (void *)list->tail;
    list->tail = (void *)other_list->tail;
    list->size += other_list->size;
}

void ll_push_back(void *self, void *value_address) {
    LinkedList(char *) *list = self;
    typedef LinkedNode(char *) bytenode;
    bytenode *new_node = malloc(sizeof(bytenode));
    memcpy(&new_node->body, value_address, list->element_size);
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = (void *)new_node;
    } else {
        new_node->prev = (void *)list->tail;
        list->tail->next = (void *)new_node;
    }
    list->tail = (void *)new_node;
    list->size++;
}

void ll_push_front(void *self, void *value_address) {
    LinkedList(char *) *list = self;
    typedef LinkedNode(char *) bytenode;
    bytenode *new_node = malloc(sizeof(bytenode));
    memcpy(&new_node->body, value_address, list->element_size);
    new_node->prev = NULL;

    if (list->head == NULL) {
        list->tail = (void *)new_node;
    } else {
        new_node->next = (void *)list->head;
        list->head->prev = (void *)new_node;
    }
    list->head = (void *)new_node;
    list->size++;
}

void *ll_pop_back(void *self) {
    LinkedList(void *) *list = self;
    if (list->tail != NULL) {
        void *result = &list->tail->body;
        list->tail = list->tail->prev;
        list->tail->next = NULL;
        list->size--;
        return result;
    } else {
        return NULL;
    }
}

void *ll_pop_front(void *self) {
    LinkedList(void *) *list = self;
    if (list->head != NULL) {
        void *result = &list->head->body;
        list->head = list->head->next;
        list->head->prev = NULL;
        list->size--;
        return result;
    } else {
        return NULL;
    }
}

const void *ll_get(const void *self, const size_t index) {
    const LinkedList(void *) *list = self;
    const LinkedNode(void *) * current;
    if (index >= list->size || index < 0) return NULL;
    if (index < list->size / 2) {
        current = (void *)list->head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        current = (void *)list->tail;
        for (size_t i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    }
    return &current->body;
}

void *ll_get_mut(void *self, size_t index) {
    LinkedList(void *) *list = self;
    LinkedNode(void *) * current;
    if (index >= list->size || index < 0) return NULL;
    if (index < list->size / 2) {
        current = (void *)list->head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
    } else {
        current = (void *)list->tail;
        for (size_t i = list->size - 1; i > index; i--) {
            current = current->prev;
        }
    }
    return &current->body;
}

InsertRes ll_insert(void *self, size_t index, void *value_address) {
    LinkedList(void *) *list = self;
    if (index == 0) {
        ll_push_front(self, value_address);
    } else if (index == list->size) {
        ll_push_back(self, value_address);
    } else {
        LinkedNode(void *) *at = (void *)list->get(list, index);
        if (at == NULL) {
            return Err(InsertRes, "Index out of range");
        } else {
            typedef LinkedNode(char *) bytenode;
            bytenode *new_node = malloc(sizeof(bytenode));
            memcpy(&new_node->body, value_address, list->element_size);

            new_node->next = (void *)at;
            new_node->prev = (void *)at->prev;
            at->prev->next = (void *)new_node;
            at->prev = (void *)new_node;

            list->size++;
        }
    }
    return Ok(InsertRes, {});
}

// TODO test
void *ll_remove(void *self, size_t index) {
    LinkedList(void *) *list = self;
    if (index == 0) {
        return ll_pop_front(self);
    } else if (index == list->size) {
        return ll_pop_back(self);
    } else {
        LinkedNode(void *) *at = (void *)list->get(list, index);
        if (at == NULL) {
            return NULL;
        } else {
            at->prev->next = at->next;
            at->next->prev = at->prev;
            list->size--;
            return at;
        }
    }
}

const LinkedListTrait LinkedListImpl = {.push_back = ll_push_back,
                                        .push_front = ll_push_front,
                                        .pop_back = ll_pop_back,
                                        .pop_front = ll_pop_front,
                                        .append = ll_append,
                                        .get = ll_get,
                                        .get_mut = ll_get_mut,
                                        .remove = ll_remove,
                                        .insert = ll_insert,
                                        .destroy = ll_destroy};
