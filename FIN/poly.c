//
// Created by jw910731 on 2021/6/5.
//
#include "poly.h"
#include "basic.h"
#include "linuxlist.h"

#include <stdint.h>
#include <stdlib.h>

void set_poly( struct list_head *pPoly, int32_t coefficient, int32_t degree ){
    struct list_head *it;
    list_for_each(it, pPoly){
        sPolyTerm *entry = list_entry(it, sPolyTerm, l);
        // overwrite
        if(entry->degree == degree){
            entry->coeff = coefficient;
            break;
        }
        // insert
        if(entry->degree < degree){
            sPolyTerm *nnd = calloc(1, sizeof(sPolyTerm));
            nnd->degree = degree;
            nnd->coeff = coefficient;
            list_add(&nnd->l, pPoly);
            break;
        }
    }
}

static char sgn(i32 a) {
    if (a > 0)
        return '+';
    return '-';
}

void print_poly( struct list_head *pPoly ){
    struct list_head *it;
    bool flag = false;
    list_for_each(it, pPoly){
        sPolyTerm *entry = list_entry(it, sPolyTerm, l);
        // sign
        if (flag) {
            putchar(sgn(entry->coeff));
            putchar(' ');
        }
        // coeff
        // this coeff is 1 or is constant coeff
        if (abs(entry->coeff) != 1 || entry->degree <= 0) {
            printf("%d", flag ? abs(entry->coeff) : entry->coeff);
        } else {
            if (!flag && entry->coeff < 0)
                putchar('-');
        }
        // power
        if (entry->degree > 0) {
            putchar('x');
        }
        if (entry->degree > 1) {
            printf("^%d", entry->degree);
        }
        putchar(' ');
        flag = true;
    }
}
void clear_poly( struct list_head *pPoly ){
    struct list_head *tmp;
    for (struct list_head *pos = (pPoly)->next; pos != (pPoly); pos = pos->next){
        tmp = pos->next;
        free(list_entry(pos, sPolyTerm, l));
    }
}