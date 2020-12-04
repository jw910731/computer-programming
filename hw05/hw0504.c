#include "analyze.h"
#include "basic.h"
#include <stdio.h>

int main() {
    i64 size;
    printf("Please enter the point number: ");
    scanf("%ld", &size);
    if (size < 0)
        err("Point number cannot smaller than 0.");
    i64 x[size], y[size];
    for (i64 i = 0; i < size; ++i) {
        printf("Please enter Point %ld: ", i + 1);
        scanf("%ld%ld", x + i, y + i);
    }
    fp cov = covariance(x, y, size), xStdSQ = stddevSQ(x, size);
    fp xMean = mean(x, size), yMean = mean(y, size);
    fp coeff = cov / xStdSQ, constant = yMean - (coeff * xMean);
    printf("Regression Equation: y = %lf x + %lf", coeff, constant);
    return 0;
}