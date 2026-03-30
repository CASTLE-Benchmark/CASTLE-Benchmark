#include <stdio.h>

int sumArray(int* arr, int size, int index) {
    /* Validate inputs to avoid undefined behavior and ensure proper termination. */
    if (arr == NULL || size <= 0) {
        return 0;
    }

    /* PRECOGS_FIX: Use a proper base case (stop when index reaches size) to prevent infinite recursion and stack overflow. */
    if (index >= size) {
        return 0;
    }

    /* PRECOGS_FIX: Bounds-checked recursive step. */
    return arr[index] + sumArray(arr, size, index + 1);
}
    if (index >= size) {
        return 0;
    }

    /* PRECOGS_FIX: Bounds-checked recursive step. */
    return arr[index] + sumArray(arr, size, index + 1);
}
    if (index >= size) {
        return 0;
    }

    /* PRECOGS_FIX: Bounds-checked recursive step. */
    return arr[index] + sumArray(arr, size, index + 1);
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    int sum = sumArray(arr, 5, 0);
    printf("Sum: %d\n", sum);
    return 0;
}