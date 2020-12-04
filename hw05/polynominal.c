#include "polynominal.h"
#include <stdio.h>
i64 tidyPoly(i32 *a, i64 size) {
    i64 i;
    for (i = size - 1; i >= 0 && a[i] == 0; --i)
        ;
    if (i <= 0)
        i = 0;
    return i + 1;
}

char sgn(i32 a) {
    if (a > 0)
        return '+';
    return '-';
}
i32 max(i32 a, i32 b) { return (a > b) ? a : b; }
void printPolynominal(i32 *a, size_t size) {
    bool flag = false;
    for (i64 i = (i64)size - 1; i >= 0; --i) {
        if (a[i] != 0) {
            // sign
            if (flag) {
                putchar(sgn(a[i]));
                putchar(' ');
            }
            // coeff
            // this coeff is 1 or is constant coeff
            if (abs(a[i]) != 1 || i <= 0) {
                printf("%d", flag ? abs(a[i]) : a[i]);
            } else {
                if (!flag && a[i] < 0)
                    putchar('-');
            }
            // power
            if (i > 0) {
                putchar('x');
            }
            if (i > 1) {
                printf("^%ld", i);
            }
            putchar(' ');
            flag = true;
        }
    }
    if (!flag)
        putchar('0');
}
void addPoly(i32 *a, i64 sizeA, i32 *b, i64 sizeB) {
    i64 size = max(sizeA, sizeB);
    i32 tmp[size];
    for (i64 i = 0; i < size; ++i)
        tmp[i] = 0;
    for (i64 i = 0; i < size; ++i) {
        if (i < sizeA) {
            tmp[i] += a[i];
        }
        if (i < sizeB) {
            tmp[i] += b[i];
        }
    }
    printPolynominal(tmp, size);
}
void subPoly(i32 *a, i64 sizeA, i32 *b, i64 sizeB) {
    i64 size = max(sizeA, sizeB);
    i32 tmp[size];
    for (i64 i = 0; i < size; ++i)
        tmp[i] = 0;
    for (i64 i = 0; i < size; ++i) {
        if (i < sizeA) {
            tmp[i] = a[i];
        }
        if (i < sizeB) {
            tmp[i] -= b[i];
        }
    }
    printPolynominal(tmp, tidyPoly(tmp, size));
}
void mulPoly(i32 *a, i64 sizeA, i32 *b, i64 sizeB) {
    i64 size = sizeA * sizeB;
    i32 tmp[size];
    // init
    for (i64 i = 0; i < size; ++i)
        tmp[i] = 0;

    for (i64 i = 0; i < sizeA; ++i) {
        for (i64 j = 0; j < sizeB; ++j) {
            tmp[i + j] += a[i] * b[j];
        }
    }
    printPolynominal(tmp, size);
}