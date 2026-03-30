#include <stdio.h>
#include <stdlib.h>

int square(int* a) {
    if (a == NULL) {
        // PRECOGS_FIX: validate input pointer to prevent NULL dereference
        return 0;
    }
    int b = (*a) * (*a);
    // PRECOGS_FIX: removed free(a) to avoid double-free; caller retains ownership
    return b;
}

int square(int* a) {
    if (a == NULL) {
        // PRECOGS_FIX: validate input pointer to prevent NULL dereference
        return 0;
    }
    int b = (*a) * (*a);
    // PRECOGS_FIX: removed free(a) to avoid double-free; caller retains ownership
    return b;
}