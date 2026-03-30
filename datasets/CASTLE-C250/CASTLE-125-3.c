#include <stdio.h>
#include <stddef.h>

int main() {
    long long int nondet[2*100000+3];
    size_t len = sizeof(nondet)/sizeof(nondet[0]); // PRECOGS_FIX: compute array length to use for safe bounds checking

    for(long long int i=0; i<=200000 && (size_t)i < len; i++) { nondet[i]=i; };

    for(size_t i=0; i<len; i++) { // PRECOGS_FIX: iterate only up to the actual array length to prevent OOB access
        printf("%lld\n", (long long)nondet[i]);
    }

    return 0;
}


    return 0;
}


    return 0;
}
