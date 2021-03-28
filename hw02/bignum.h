//
// Created by jw910731 on 3/27/21.
//

#ifndef HW02_BIGNUM_H
#define HW02_BIGNUM_H

#include <stdint.h>

#ifndef GC_ENABLE
#define MALLOC(x) malloc(x)
#define FREE(x) free(x)
#define REALLOC(x) realloc(x)
#else
#include <gc.h>
#define MALLOC(x) GC_MALLOC(x)
#define FREE(x) GC_FREE(x)
#define REALLOC(x, size) GC_REALLOC(x, size)
#endif  // ifndef GC_ENABLE

typedef struct _sBigNum {
    char *n;
} sBigNum;

void print(const sBigNum num);

int32_t set(sBigNum *pNum, char *str);

// return -1 0 1 represent less than, equal than, greater than
int32_t compare(const sBigNum num01, const sBigNum num02);

int32_t digits(const sBigNum num);

void add(sBigNum *pResult, const sBigNum num01, const sBigNum num02);

void subtract(sBigNum *pResult, const sBigNum num01, const sBigNum num02);

void multiply(sBigNum *pResult, const sBigNum num01, const sBigNum num02);

void divide(sBigNum *pQuotient, sBigNum *pRemainder, const sBigNum num01,
            const sBigNum num02);

void power(sBigNum *pResult, int32_t n, int32_t k);

void combine(sBigNum *pResult, int32_t n, int32_t k);

#endif  // HW02_BIGNUM_H
