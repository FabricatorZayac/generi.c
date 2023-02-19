#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "vector.h"

DefineLinkedList(LinkedListInt, int);
void ll_test() {
    LinkedListInt foo = LinkedListInt_.new();

    /* foo.push_back(&foo, 6); */
    foo.push_back(&foo, 5);
    /* foo.push_back(&foo, 4); */
    foo.push_back(&foo, 3);
    foo.push_back(&foo, 2);
    foo.push_front(&foo, 6);
    /* foo.insert(&foo, 0, 6); */
    foo.insert(&foo, 2, 4);
    /* { */
    /*     LinkedNode(int) *i = (void *)foo.head; */
    /*     for (; i != NULL; i = i->next) { */
    /*         printf("%d\n", i->body); */
    /*     } */
    /*     printf("\n"); */
    /*     i = (void *)foo.tail; */
    /*     for (; i != NULL; i = i->prev) { */
    /*         printf("%d\n", i->body); */
    /*     } */
    /*     printf("\n"); */
    /* } */

    assert(foo.get(&foo, 5) == NULL);

    assert(foo.get(&foo, 1) != NULL);
    assert(*foo.get(&foo, 1) == 5);

    int *a = foo.pop_front(&foo);
    assert(a != NULL);
    assert(*a == 6);
    free(a);

    a = foo.pop_front(&foo);
    assert(a != NULL);
    assert(*a == 5);
    free(a);

    LinkedListInt bar = LinkedListInt_.new();

    bar.push_back(&bar, 100);
    bar.push_back(&bar, 200);
    bar.push_front(&bar, 9000);
    bar.push_front(&bar, 123);
    int *arr = (int[]) {123, 9000, 100, 200};
    for (size_t i = 0; i < bar.size; i++) {
        /* printf("%6d|%6d\n", arr[i], *bar.get(&bar, i)); */
        assert(arr[i] == *bar.get(&bar, i));
    }

    foo.impl.append(&foo, &bar);
    a = foo.pop_front(&foo);
    assert(a != NULL);
    assert(*a == 4);
    free(a);

    a = foo.pop_back(&foo);
    assert(a != NULL);
    assert(*a == 200);
    free(a);

    printf("\n");
    arr = (int[]) {3, 2, 123, 9000, 100};
    for (size_t i = 0; i < foo.size; i++) {
        /* printf("%6d|%6d\n", arr[i], *foo.get(&foo, i)); */
        assert(arr[i] == *foo.get(&foo, i));
    }

    a = foo.remove(&foo, 2);
    assert(a != NULL);
    assert(*a == 123);
    free(a);

    arr = (int[]) {3, 2, 9000, 100};
    for (size_t i = 0; i < foo.size; i++) {
        assert(arr[i] == *foo.get(&foo, i));
    }

    foo.impl.destroy(&foo);
}

DefineVec(VecInt, int);
void vec_test() {
    VecInt foo = VecInt_.new();

    foo.push_back(&foo, 6);
    foo.push_back(&foo, 5);
    foo.push_back(&foo, 4);
    foo.push_back(&foo, 3);
    foo.push_back(&foo, 2);
    assert(foo.capacity == 8);
    assert(foo.size == 5);

    /* for (size_t i = 0; i < foo.size; i++) { */
    /*     printf("%d, ", *foo.get(&foo, i)); */
    /* } */
    /* puts(""); */

    assert(memcmp(foo.body, (int[]) {6, 5, 4, 3, 2}, foo.size * sizeof(int))
           == 0);

    foo.impl.shrink_to(&foo, 6);
    assert(foo.capacity == 6);

    foo.impl.shrink_to_fit(&foo);
    assert(foo.capacity == 5);

    foo.put(&foo, 2, 2000);
    assert(*foo.get(&foo, 2) == 2000);
    assert(errno == 0);

    foo.put(&foo, 10, 4);
    assert(errno != 0);

    assert(foo.get(&foo, 10) == NULL);
    assert(foo.impl.get(&foo, 10) == NULL);

    int *a = foo.pop_back(&foo);
    assert(*a == 2);
    assert(foo.size == 4);
    free(a);

    assert(memcmp(foo.body, (int[]) {6, 5, 2000, 3}, foo.size * sizeof(int))
           == 0);

    foo.impl.shrink_to_fit(&foo);
    assert(foo.capacity == 4);

    foo.impl.reserve(&foo, 5);
    assert(foo.capacity == 9);

    foo.impl.truncate(&foo, 2);
    assert(foo.capacity == 2);
    assert(foo.size == 2);

    int arr[] = {7, 5000, 3, 4};
    VecInt bar = VecInt_.from_arr(arr, sizeof(arr));
    assert(memcmp(bar.body, arr, sizeof(arr)) == 0);
    assert(bar.size == sizeof(arr) / sizeof(int));
    assert(bar.capacity == sizeof(arr) / sizeof(int));

    a = bar.pop_front(&bar);
    assert(*a == 7);
    free(a);
    assert(*bar.get(&bar, 0) == 5000);

    bar.push_front(&bar, 57);
    assert(*bar.get(&bar, 0) == 57);

    bar.impl.append(&bar, &foo);
    int arr1[] = {57, 5000, 3, 4, 6, 5};

    assert(memcmp(bar.body, arr1, bar.size * sizeof(int)) == 0);
    assert(sizeof(int) == foo.element_size);

    bar.remove(&bar, 1);
    assert(memcmp(bar.body, (int[]) {57, 3, 4, 6, 5}, bar.size * sizeof(int))
           == 0);

    bar.insert(&bar, 3, 1000);
    assert(
        memcmp(bar.body, (int[]) {57, 3, 4, 1000, 6, 5}, bar.size * sizeof(int))
        == 0);

    foo.impl.destroy(&foo);
    bar.impl.destroy(&bar);
}

int main(int argc, char *argv[]) {
    vec_test();
    ll_test();
    return 0;
}
