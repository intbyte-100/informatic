#include <stdlib.h>
#include <time.h>

static int compare(const void *a, const void *b) {
    double da = *(const double *)a;
    double db = *(const double *)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

static double find_median(double *numbers, int size) {
    if (size == 0) return 0.0;

    qsort(numbers, size, sizeof(double), compare);
    
    if (size % 2 == 0) {
        return (numbers[size / 2 - 1] + numbers[size / 2]) / 2.0;
    } else {
        return numbers[size / 2];
    }
}

double test_median_n_times(double *numbers, int size, int times) {
    double start_time = clock();
    
    for (int i = 0; i < times; i++) {
        find_median(numbers, size);
    }
    
    return (double) ((double) clock() - start_time) / CLOCKS_PER_SEC;
}