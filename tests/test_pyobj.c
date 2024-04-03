#include "pyas/all.h"
#include "pyas/pyobj.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    pyobj_t object = (pyobj_t)malloc(sizeof(struct pyobj));
    object->refcount = 1;

    // String
    object->type = STRING_TYPE;
    object->py.string.buffer = strdup("Hello World!");
    object->py.string.length = strlen(object->py.string.buffer);
    printf("Printing a string:\n");
    print_pyobj_t(object);
    printf("\n");

    // Integer
    object->type = INT_TYPE;
    object->py.number.integer = 42;
    printf("Printing an integer:\n");
    print_pyobj_t(object);
    printf("\n");

    // Float
    object->type = FLOAT_TYPE;
    object->py.number.real = 3.14;
    printf("Printing a float:\n");
    print_pyobj_t(object);
    printf("\n");

    // Complex
    object->type = COMPLEX_TYPE;
    object->py.number.complex.real = 3.14;
    object->py.number.complex.imag = 2.71;
    printf("Printing a complex:\n");
    print_pyobj_t(object);
    printf("\n");

    // List
    object->type = LIST_TYPE;
    object->py.list.size = 3;
    object->py.list.value = (pyobj_t *)malloc(sizeof(pyobj_t) * object->py.list.size);
    printf("Printing a list:\n");
    print_pyobj_t(object);
    printf("\n");

    free(object);

    return 0;
}
