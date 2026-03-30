#include <stdio.h>
#include <stdlib.h>

void square(int* a) {
    if (a == NULL) {
        return; // PRECOGS_FIX: check for NULL to avoid dereference of a null pointer
    }
    int b = *a;
    *a = b * b;
    // PRECOGS_FIX: removed free(a); preserve ownership with the caller to avoid use-after-free
}
    *a = b * b;
    // PRECOGS_FIX: removed free(a); preserve ownership with the caller to avoid use-after-free
}
    *a = b * b;
    // PRECOGS_FIX: removed free(a); preserve ownership with the caller to avoid use-after-free
}

int main() {
    int *a = (int*) malloc(sizeof(int));

    if (a == NULL) {
        printf("Failed to allocate memory.\n");
        return 1; // Indicate error and exit
    }

    *a = 5;
    square(a);
    printf("Result: %d\n", *a);

    return 0;
}