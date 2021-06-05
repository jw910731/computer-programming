#include <stdint.h>
#include "linuxlist.h"
#include "basic.h"

#pragma once

/*
 * In descend order
 */
typedef struct _sPolyTerm{
    struct list_head l;
    i32 coeff, degree;
} sPolyTerm;

void set_poly( struct list_head *pPoly, int32_t coefficient, int32_t degree );
void print_poly( struct list_head *pPoly );
void clear_poly( struct list_head *pPoly );
void add_poly( struct list_head *pResult, struct list_head *pPoly1, struct list_head *pPoly2 );
void del_poly( struct list_head *pResult, struct list_head *pPoly1, struct list_head *pPoly2 );
void mul_poly( struct list_head *pResult, struct list_head *pPoly1, struct list_head *pPoly2 );
void pow_poly( struct list_head *pResult, struct list_head *pPoly, int32_t n );
int32_t eval_poly( struct list_head *pPoly, int32_t a );
void diff_poly( struct list_head *pResult, struct list_head *pPoly );
double int_poly( struct list_head *pPoly, int32_t a, int32_t b );
