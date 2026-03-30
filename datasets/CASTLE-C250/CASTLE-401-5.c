#include <stdio.h>
#include <stdlib.h>

int square(int* a) {
    int b = *a * *a;
    return b;
}

void swap(int** a, int** b) {
    if (a == NULL || b == NULL) {
        fprintf(stderr, "swap: null pointer-to-pointer argument\n");
        return; // PRECOGS_FIX: validate pointer-to-pointer arguments to avoid dereferencing NULL
    }
    int* temp = *a;
    *a = *b;
    *b = temp;
}

void safeFree(int** a) {
    free(*a);
    *a = NULL;
}

int main() {
    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));
    if (a == NULL || b == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        if (a != NULL) { free(a); a = NULL; } // PRECOGS_FIX: free any partially allocated resource 'a'
        if (b != NULL) { free(b); b = NULL; } // PRECOGS_FIX: free any partially allocated resource 'b'
        return 1;
    }

    *a = 5;
    printf("Result: %d\n", square(a));

    *b = -7;
    printf("Result: %d\n", square(b));

    /* Properly free both allocated objects. Avoid swapping freed pointers which can lead
       to difficult-to-follow ownership and potential leaks. */
    safeFree(&a);
    safeFree(&b);

    return 0;
}