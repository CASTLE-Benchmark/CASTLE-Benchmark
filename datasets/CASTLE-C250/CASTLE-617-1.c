#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void correctLengths(double* x, double* y, double* z) {
    /* PRECOGS_FIX: swap the values pointed to by the pointers rather than reassigning the local pointer variables */
    double s = *x; // PRECOGS_FIX: perform actual value rotation
    *x = *y;
    *y = *z;
    *z = s;
}

UnitVector createUnitVector(double x, double y, double z) {
    correctLengths(&x, &y, &z);
    UnitVector uv = {x, y, z};
    return uv;
}

int isValidUnitVector(UnitVector uv) {
    /* PRECOGS_FIX: use epsilon tolerance to account for floating-point rounding errors */
    double sum = uv.x * uv.x + uv.y * uv.y + uv.z * uv.z;
    double diff = sum - 1.0;
    if (diff < 0) diff = -diff;
    return diff <= 1e-9; // tolerance for floating-point imprecision
}

int main() {
    UnitVector uv = createUnitVector(1.0, 2.0, 3.0);
    /* PRECOGS_FIX: replace assert with a runtime check and graceful error handling to avoid aborting the process */
    if (!isValidUnitVector(uv)) {
        fprintf(stderr, "Invalid unit vector: {%f, %f, %f}\n", uv.x, uv.y, uv.z);
        return EXIT_FAILURE;
    }

    printf("{%f, %f, %f}\n", uv.x, uv.y, uv.z);

    return 0;
}