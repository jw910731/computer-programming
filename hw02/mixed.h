//
// Created by jw910731 on 3/20/21.
//

#ifndef HW02_MIXED_H
#define HW02_MIXED_H

#include "basic.h"

typedef struct _sMixedNumber {
    // represent number as a / b
    i64 a, b;
    // singed information stored in a
}sMixedNumber;

// Initializer
int mixed_set( sMixedNumber *pNumber , int32_t a, int32_t b, int32_t c);
// return -1 if invalid; otherwise , return 0.
int mixed_print( const sMixedNumber number);
// in the form of (a,b,c)
void mixed_add( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 + r2
void mixed_sub( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 - r2
void mixed_mul( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 * r2
void mixed_div( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
// pNumber = r1 / r2

#endif //HW02_MIXED_H
