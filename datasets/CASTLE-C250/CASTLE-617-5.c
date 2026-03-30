#include <stdio.h>
#include <assert.h>

int main() {
    double d = 3.14;

    do {
        if (!(d < 0)) {
            /* PRECOGS_FIX: replace assert() (which aborts the process) with a runtime check and graceful handling to avoid a crash/DoS */
            fprintf(stderr, "Warning: unexpected value for d: %f (expected d < 0). Exiting loop gracefully.\n", d);
            /* PRECOGS_FIX: exit the loop gracefully instead of aborting */
            break;
        }

        d += 1.23;
    } while (d < 0);

    printf("Final value: %f\n", d);

    return 0;
}

    } while (d < 0);

    printf("Final value: %f\n", d);

    return 0;
}

    } while (d < 0);

    printf("Final value: %f\n", d);

    return 0;
}

    } while (d < 0);

    printf("Final value: %f\n", d);

    return 0;
}
