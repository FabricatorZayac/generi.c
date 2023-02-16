#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>

#include "vector.h"

DefineVec(VecInt, int);

void vec_test() {
    VecInt foo = VecInt_.new();

    foo.push(&foo, 6);
    foo.push(&foo, 5);
    foo.push(&foo, 4);
    foo.push(&foo, 3);
    foo.push(&foo, 2);
    assert(foo.capacity == 8);
    assert(foo.size == 5);

    int arr[] = {6, 5, 4, 3, 2};
    assert(memcmp(foo.body, arr, foo.size * sizeof(int)) == 0);
    for (size_t i = 0; i < foo.size; i++) {
        assert(*foo.get(&foo, i) == arr[i]);
    }

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
    assert(foo.impl.list.get(&foo, 10) == NULL);

    assert(*foo.pop(&foo) == 2);
    assert(foo.size == 4);

    foo.impl.shrink_to_fit(&foo);
    assert(foo.capacity == 4);

    foo.impl.reserve(&foo, 5);
    assert(foo.capacity == 9);

    foo.impl.truncate(&foo, 4);
    assert(foo.capacity == 4);
    assert(foo.size == 4);

    foo.impl.destroy(&foo);

    VecInt bar = VecInt_.from_arr(arr, sizeof(arr));
    assert(memcmp(bar.body, arr, sizeof(arr)) == 0);
    assert(bar.size == 5);
    assert(bar.capacity == 5);
}

int main(int argc, char *argv[]) {
    vec_test();
    return 0;
}
