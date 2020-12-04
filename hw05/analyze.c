#include "analyze.h"

fp mean(i64 *a, size_t size) {
    i64 sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += a[i];
    }
    return sum / size;
}

fp stddevSQ(i64 *a, size_t size) {
    fp mn = mean(a, size), sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += ((mn - a[i]) * (mn - a[i]));
    }
    return sum / size;
}

fp covariance(i64 *a, i64 *b, size_t size) {
    fp aMean = mean(a, size), bMean = mean(b, size);
    // calculate covariance
    fp cov = 0;
    for (size_t i = 0; i < size; ++i) {
        cov += (a[i] - aMean) * (b[i] - bMean);
    }
    return cov / size;
}