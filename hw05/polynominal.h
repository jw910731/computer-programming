#pragma once

#include "basic.h"
#include <stdlib.h>

// polynominal is defined in order

void printPolynominal(i32 *a, size_t size);
i64 tidyPoly(i32 *a, i64 size);

// theese computational function do not return, instead they flush the
// answer to stdout due to my laziness
void addPoly(i32 *a, i64 sizeA, i32 *b, i64 sizeB);
void subPoly(i32 *a, i64 sizeA, i32 *b, i64 sizeB);
void mulPoly(i32 *a, i64 sizeA, i32 *b, i64 sizeB);
