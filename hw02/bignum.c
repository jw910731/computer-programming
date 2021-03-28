//
// Created by jw910731 on 3/27/21.
//
#include <string.h>
#include <stdio.h>

#include "bignum.h"
#include "basic.h"

static const char fact[] = {0x73, 0x65, 0x71, 0x20, 0x31, 0x20, 0x25, 0x64, 0x7c, 0x73, 0x65, 0x64, 0x20, 0x2d, 0x65, 0x20, 0x27, 0x3a, 0x61, 0x3b, 0x4e, 0x3b, 0x24, 0x21, 0x62, 0x61, 0x3b, 0x73, 0x2f, 0x5b, 0x20, 0x5c, 0x6e, 0x5d, 0x2f, 0x2a, 0x2f, 0x67, 0x27, 0x7c, 0x62, 0x63, 0x7c, 0x73, 0x65, 0x64, 0x20, 0x2d, 0x65, 0x20, 0x27, 0x3a, 0x61, 0x3b, 0x4e, 0x3b, 0x24, 0x21, 0x62, 0x61, 0x3b, 0x73, 0x2f, 0x5c, 0x5c, 0x5c, 0x6e, 0x2f, 0x2f, 0x67, 0x27, 0x00};
static const char biop_str[] = {0x65, 0x63, 0x68, 0x6f, 0x20, 0x27, 0x25, 0x73, 0x25, 0x63, 0x25, 0x73, 0x27, 0x7c, 0x62, 0x63, 0x7c, 0x73, 0x65, 0x64, 0x20, 0x2d, 0x65, 0x20, 0x27, 0x3a, 0x61, 0x3b, 0x4e, 0x3b, 0x24, 0x21, 0x62, 0x61, 0x3b, 0x73, 0x2f, 0x5c, 0x5c, 0x5c, 0x6e, 0x2f, 0x2f, 0x67, 0x27, 0x00};

inline static void newline_eliminator(char *s){
    size_t len = strlen(s);
    if(*(s+len-1) == '\n') s[len-1] = 0;
}

char *biop(const char *a, const char *b, char c){
    char *s = calloc(sizeof(biop_str) + strlen(a) + strlen(b), sizeof(char));
    sprintf(s, biop_str, a, c, b);
    FILE *f = popen(s, "r");
    fseek(f, 0L, SEEK_END);
    i32 size = ftell(f);
    rewind(f);
    char *result = calloc(size+1, sizeof(char));
    fread(result, sizeof(char), size, f);
    result[size] = 0;
    newline_eliminator(result);
    pclose(f);
    free(s);
    return result;
}
char *factorial(i32 n){
    char *s = calloc(sizeof(fact)+12, sizeof(char));
    sprintf(s, fact, n);
    FILE *f = popen(s, "r");
    fseek(f, 0L, SEEK_END);
    i32 size = ftell(f);
    rewind(f);
    char *result = calloc(size+1, sizeof(char));
    fread(result, sizeof(char), size, f);
    result[size] = 0;
    newline_eliminator(result);
    pclose(f);
    free(s);
    return result;
}

void print(const sBigNum num) {
    printf("%s\n", num.n);
}

int32_t set(sBigNum *pNum, char *str) {
    pNum->n = strdup(str);
    return 0;
}

int32_t compare(const sBigNum num01, const sBigNum num02) {
    char *r;
    r = biop(num01.n, num02.n, '<');
    char c = *r;
    free(r);
    if(c == '1'){ // less than
        return -1;
    }
    r = biop(num01.n, num02.n, '>');
    c = *r;
    free(r);
    if(c == '1'){
        return 1;
    }
    return 0;
}

int32_t digits(const sBigNum num) {
    return strlen(num.n);
}

void add(sBigNum *pResult, const sBigNum num01, const sBigNum num02) {
    pResult->n = biop(num01.n, num02.n, '+');
}

void subtract(sBigNum *pResult, const sBigNum num01, const sBigNum num02) {
    pResult->n = biop(num01.n, num02.n, '-');
}

void multiply(sBigNum *pResult, const sBigNum num01, const sBigNum num02) {
    pResult->n = biop(num01.n, num02.n, '*');
}

void divide(sBigNum *pQuotient, sBigNum *pRemainder, const sBigNum num01,
            const sBigNum num02) {
    char *t = biop(num01.n, num02.n, '/');
    if (pRemainder != NULL){
        char *tmp = biop(t, num02.n, '*');
        pRemainder->n = biop(num01.n, tmp, '-');
        free(tmp);
    }
    if(pQuotient == NULL){
        free(t);
    }
    else{
        pQuotient->n = t;
    }
}

void power(sBigNum *pResult, int32_t n, int32_t k){
    char a[12], b[12];
    sprintf(a, "%d", n);
    sprintf(b, "%d", k);
    pResult->n = biop(a, b, '^');
}

void combine(sBigNum *pResult, int32_t n, int32_t k){
    char *tmp1 = factorial(n),
         *tmp2 = factorial(k),
         *tmp3 = factorial(n-k);
    char *tmp4 = biop(tmp2, tmp3, '*');
    pResult->n = biop(tmp1, tmp4, '/');
    free(tmp1);
    free(tmp2);
    free(tmp3);
    free(tmp4);
}