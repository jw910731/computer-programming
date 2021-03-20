//
// Created by jw910731 on 3/20/21.
//

#include "mixed.h"

#include <stdio.h>
#include <stdlib.h>

static inline i64 lsgn(i64 a){
    return (a>0)-(a<0);
}

static inline i64 gcd(i64 a, i64 b){
    if (b)
        while((a %= b) && (b %= a));
    return a + b;
}

static inline i64 lcm(i64 a, i64 b){
    return a * b / gcd(a, b);
}

static inline void simplify(sMixedNumber *num){
    i64 g = gcd(abs(num->a), abs(num->b));
    if(g) {
        num->a = num->a / g;
        num->b = num->b / g;
    }
    if(lsgn(num->a) == lsgn(num->b) && num->b < 0){
        num->a *= -1;
        num->b *= -1;
    }
}

int mixed_set( sMixedNumber *pNumber , int32_t a, int32_t b, int32_t c){
    if((a!=0 && (b<0 || c<0)) || (b != 0 && !c)) return -1;
    if(!c) c = 1;
    pNumber->a = ((i64)(a)*(i64)(c));
    pNumber->a += ((pNumber->a != 0)?lsgn(pNumber->a):1)*(i64)b;
    pNumber->b = c;
    simplify(pNumber);
    return 0;
}
// return -1 if invalid; otherwise , return 0.

int mixed_print( const sMixedNumber number){
    i64 a, b, c;
    a = number.a/number.b;
    // b is negative only when a is zero
    b = ((a != 0)?labs(number.a):number.a)%number.b;
    c = (b != 0)?number.b:0;
    printf("(%lld, %lld, %lld)", a, b, c);
    return 0;
}
// in the form of (a,b,c)

// Num -> sMixedNumber, NB -> new base
// Extend fractional to new base
#define EXT(NUM, NB) (NUM.a * ((NB)/NUM.b))

void mixed_add( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2){
    i64 l = pNumber->b = lcm(r1.b, r2.b);
    pNumber->a = EXT(r1, l) + EXT(r2, l);
    simplify(pNumber);
}
// pNumber = r1 + r2

void mixed_sub( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2){
    i64 l = pNumber->b = lcm(r1.b, r2.b);
    pNumber->a = EXT(r1, l) - EXT(r2, l);
    simplify(pNumber);
}
// pNumber = r1 - r2

void mixed_mul( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2){
    pNumber->a = r1.a * r2.a;
    pNumber->b = r1.b * r2.b;
    simplify(pNumber);
}
// pNumber = r1 * r2

void mixed_div( sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2){
    pNumber->a = r1.a * r2.b;
    pNumber->b = r2.a * r1.b;
    simplify(pNumber);
}
// pNumber = r1 / r2