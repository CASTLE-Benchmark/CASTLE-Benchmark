#include <stdio.h>
#include <stdlib.h>

int* getPointer() {
    int* val = (int*) malloc(sizeof(int));
    if (val == NULL) {
        return NULL;
    }

    *val = 42;
    return val;
}

int main() {
    int *a = getPointer();
    if (a == NULL) {
        fprintf(stderr, "Failed to get pointer\n");
        return 1;
    }

    printf("Value: %d\n", *a);

    free(a);

    return 0;
}