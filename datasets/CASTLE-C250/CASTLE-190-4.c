#include <stdio.h>
#include <limits.h>

int main() {
    int n = 214748364;

    /* Compute using a wider type and check for overflow before converting back to int */
    long long temp = (long long)n * 10 + 8; // PRECOGS_FIX: use wider type to perform arithmetic safely
    if (temp > INT_MAX || temp < INT_MIN) {
        fprintf(stderr, "Integer overflow detected: operation would exceed int range\n"); // PRECOGS_FIX: detect overflow and fail safely
        return 1;
    }

    printf("%d\n", n);
    printf("%d\n", (int)temp);

    return 0;
}
