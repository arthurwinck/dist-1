#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int *array;
    size_t used;
    size_t size;
} Array;

void initArray(Array *a, size_t initialSize) {
    a->array = (int*)malloc(initialSize * sizeof(int));
    a->used = 0;
    a->size = initialSize;
}

int insertArray(Array *a, int element) {
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size 
    if (a->array != NULL) {
        if (a->used == a->size) {
            a->size *= 2;
            a->array = (int*)realloc(a->array, a->size * sizeof(int));
        }
        a->array[a->used++] = element;
        return 1;
    } else {
        return 0;
    }
}

int freeArray(Array *a) {
    if (a->array != NULL) {
        free(a->array);
        a->array = NULL;
        a->used = a->size = 0;
        return 1;
    } else {
        return 0;
    }
}