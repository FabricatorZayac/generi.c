#include "vector.h"
#include <errno.h>
#include <assert.h>
#include <string.h>

DefineVec(VecInt, int);

int main(int argc, char *argv[]) {
    VecInt a = VecInt_.new();

    a.push(&a, 6);
    a.push(&a, 5);
    a.push(&a, 4);
    a.push(&a, 3);
    a.push(&a, 2);
    assert(a.capacity == 8);
    assert(a.size == 5);

    int *arr = (int[]){6, 5, 4, 3, 2};
    assert(memcmp(a.body, arr, a.size * sizeof(int)) == 0);

    Vec.shrink_to(&a, 7);
    assert(a.capacity == 7);
    Vec.shrink_to_fit(&a);
    assert(a.capacity == 5);

    a.put(&a, 0, 2);
    assert(*a.get(&a, 0) == 2);
    assert(!strcmp("Success", strerror(errno)));

    a.put(&a, 10, 4);
    assert(!strcmp("Numerical result out of range", strerror(errno)));

    int *foo = a.get(&a, 10);
    assert(foo == NULL);

    foo = a.pop(&a);
    assert(*foo == 2);
    assert(a.size == 4);

    Vec.shrink_to_fit(&a);
    assert(a.capacity == 4);

    Vec.reserve(&a, 5);
    assert(a.capacity == 9);

    Vec.destroy(&a);

    return 0;
}
