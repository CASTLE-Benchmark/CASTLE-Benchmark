#include <stdio.h>
#include <stdlib.h>

void square(int* a) {
    int b = *a;
    *a = b * b;
}

void swap(int** a, int** b) {
    int* temp = *a;
    *a = *b;
    *b = temp;
}

void negate(int* a) {
    *a = -(*a);
}

int main() {
    int *a = (int*) malloc(sizeof(int));
    int *b = (int*) malloc(sizeof(int));

    if (a == NULL || b == NULL) {
        free(a);
        free(b);
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    *a = 5;
    square(a);
    printf("Result: %d\n", *a);
    free(a);

    swap(&a, &b);

    *a = 3;
    negate(a);
    printf("Result: %d\n", *a);

    free(a);

    return 0;
}