#include <stdio.h>
#include <stdlib.h>

void dosth() {
    int *A = (int*) malloc(sizeof(int) * 10);
    if (A == NULL) {
        perror("Could not reserve memory!");
        return;
    }

    for (int i = 1; i < 4; i++) {
        A[i] = i;
    }

    printf("%d", A[1]);

    free(A); // PRECOGS_FIX: free allocated memory to prevent memory leak
    A = NULL; // PRECOGS_FIX: avoid dangling pointer after free
}

int main() {
    dosth();
    return 0;
}