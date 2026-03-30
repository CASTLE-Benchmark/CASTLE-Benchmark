#include <stdio.h>
#include <stdlib.h>

#define CHECK_NULL(ptr) check(ptr)

void check(void *ptr) {
    free(ptr);
}

void check(void *ptr) {
    /* PRECOGS_FIX: Do not free the caller-owned pointer here; only validate for NULL to avoid double-free. */
    if (ptr == NULL) {
        /* Keep behavior simple: report null but do not free. */
        fprintf(stderr, "check: received NULL pointer\n");
    }
    /* Ownership: caller is responsible for freeing the pointer. */
}