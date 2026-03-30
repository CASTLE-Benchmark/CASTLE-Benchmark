#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int main() {
    int n = 3;
    printf("Enter 3 numbers (maximum 3 digits each), one per line:\n");

    int *arr = malloc(n * sizeof(int));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    } // PRECOGS_FIX: Check malloc() return to avoid NULL dereference

    char buf[64];
    for (int i = 0; i < n; i++) {
        long val;
        char *endptr;
        while (1) {
            if (fgets(buf, sizeof(buf), stdin) == NULL) {
                fprintf(stderr, "Input error or unexpected EOF\n");
                free(arr);
                return 1;
            }
            errno = 0;
            val = strtol(buf, &endptr, 10);
            if (endptr == buf) {
                printf("Invalid input, please enter a numeric value:\n");
                continue;
            }
            if (errno == ERANGE || val < -999 || val > 999) {
                printf("Number out of allowed range (-999..999). Try again:\n");
                continue;
            }
            /* ensure at most 3 digits (excluding sign and leading whitespace) */
            char *p = buf;
            while (*p == ' ' || *p == '\t') p++;
            if (*p == '+' || *p == '-') p++;
            int digit_count = 0;
            while (p < endptr && *p >= '0' && *p <= '9') { digit_count++; p++; }
            if (digit_count > 3) {
                printf("Too many digits (max 3). Try again:\n");
                continue;
            }
            break;
        }
        arr[i] = (int)val; // PRECOGS_FIX: Use fgets+strtol with validation instead of unchecked scanf
    }

    printf("The elements of the array are:");
    for (int i = 0; i < n; i++) {
        printf(" %d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}

        char *endptr;
        while (1) {
            if (fgets(buf, sizeof(buf), stdin) == NULL) {
                fprintf(stderr, "Input error or unexpected EOF\n");
                free(arr);
                return 1;
            }
            errno = 0;
            val = strtol(buf, &endptr, 10);
            if (endptr == buf) {
                printf("Invalid input, please enter a numeric value:\n");
                continue;
            }
            if (errno == ERANGE || val < -999 || val > 999) {
                printf("Number out of allowed range (-999..999). Try again:\n");
                continue;
            }
            /* ensure at most 3 digits (excluding sign and leading whitespace) */
            char *p = buf;
            while (*p == ' ' || *p == '\t') p++;
            if (*p == '+' || *p == '-') p++;
            int digit_count = 0;
            while (p < endptr && *p >= '0' && *p <= '9') { digit_count++; p++; }
            if (digit_count > 3) {
                printf("Too many digits (max 3). Try again:\n");
                continue;
            }
            break;
        }
        arr[i] = (int)val; // PRECOGS_FIX: Use fgets+strtol with validation instead of unchecked scanf
    }

    printf("The elements of the array are:");
    for (int i = 0; i < n; i++) {
        printf(" %d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}

