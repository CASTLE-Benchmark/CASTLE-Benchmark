#include <stdio.h>

int sumArray(int* arr, int size, int index) {
    if (index >= size) { // PRECOGS_FIX: check for out-of-bounds access
        return 0;
    }

    return arr[index] + sumArray(arr, size, index+1); // PRECOGS_FIX: ensure index does not exceed size
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    sumArray(arr, 5, 0);
    printf("Sum: %d\n", sumArray(arr, 5, 0));
    return 0;
}