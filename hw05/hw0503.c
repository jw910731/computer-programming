#include "basic.h"
#include "polynominal.h"

#include <stdio.h>

int main() {
    i64 deg1, deg2;
    printf("Please enter p1 degree: ");
    scanf("%ld", &deg1);
    if (deg1 < 0)
        err("Polynominal Degree < 0 is not allowed.");
    ++deg1;
    i32 poly1[deg1];
    printf("Please enter p1 coefficients:");
    for (i64 i = deg1 - 1; i >= 0; --i) {
        scanf("%d", poly1 + i);
    }
    printf("Please enter p2 degree: ");
    scanf("%ld", &deg2);
    if (deg2 < 0)
        err("Polynominal Degree < 0 is not allowed.");
    ++deg2;
    i32 poly2[deg2];
    printf("Please enter p2 coefficients:");
    for (i64 i = deg2 - 1; i >= 0; --i) {
        scanf("%d", poly2 + i);
    }
    deg1 = tidyPoly(poly1, deg1);
    deg2 = tidyPoly(poly2, deg2);
    debug("%ld %ld\n", deg1, deg2);
    printf("p1: ");
    printPolynominal(poly1, deg1);
    putchar('\n');
    printf("p2: ");
    printPolynominal(poly2, deg2);
    putchar('\n');
    printf("p1 + p2: ");
    addPoly(poly1, deg1, poly2, deg2);
    putchar('\n');
    printf("p1 - p2: ");
    subPoly(poly1, deg1, poly2, deg2);
    putchar('\n');
    printf("p1 * p2: ");
    mulPoly(poly1, deg1, poly2, deg2);
    putchar('\n');
    return 0;
}