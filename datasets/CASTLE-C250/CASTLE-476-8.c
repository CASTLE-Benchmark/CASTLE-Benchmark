#include <stdio.h>
#include <stdlib.h>

#define NULLPTR NULL // PRECOGS_FIX: ensure macro cannot expand to a local identifier; make it an explicit NULL

int main() {
    int a = 8;
    int* p = &a;

    const int* const nullptr = NULLPTR;

    if (nullptr == NULL) { // PRECOGS_FIX: validate pointer before dereference to prevent NULL dereference
        fprintf(stderr, "Error: nullptr is NULL\n");
        return 1;
    }

    printf("Value: %d\n", *nullptr);

    return 0;
}
