#include <stdio.h>
#include <stdlib.h>

void doCalcWithoutFree(int* a) {
    /* Validate input before dereference */
    if (a == NULL) {
        fprintf(stderr, "doCalcWithoutFree: null pointer\n");
        return;
    }

    int b = *a * *a;
    printf("Result: %d\n", b);

    /* PRECOGS_FIX: Removed freeing of caller-owned memory. Caller retains ownership and must free when appropriate. */
}

int main() {
    int *a = (int*) malloc(sizeof(int));

    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    int* p = a;
    *a = 5;
    *p = 6;

    /* Call once. Do not reuse aliases after the callee may free the memory. */
    doCalcWithoutFree(a);

    /* PRECOGS_FIX: Clear local aliases after passing the pointer to a callee that may free it, preventing use-after-free. */
    p = NULL;
    a = NULL;

    return 0;
}