//
// Created by jw910731 on 3/20/21.
//

#include <stdio.h>
#include <string.h>

#include "basic.h"

union soviet{
    double f;
    struct intern{
        u64 frac : 52;
        u64 exp : 11;
        u64 sgn : 1;
    } trans;
};

int main(){
    static char buf[1024];
    union soviet fp;
    char *ed = buf;
    size_t l = 0;
    do {
        if(ed != buf+l)
            printf("Input Invalid. \n");
        printf("Please enter a floating -point number (double precision): ");
        fgets_n(buf, 1024, stdin);
        l = strlen(buf);
        fp.f = strtod(buf, &ed);
    } while (buf+l != ed);
    printf("Sign: %d\n", fp.trans.sgn);
    printf("Exponent: ");
    for(i64 i = 10 ; i >= 0 ; --i){
        putchar('0'+((fp.trans.exp&(1<<i))>>i));
    }
    putchar('\n');
    printf("Fraction: ");
    for(i64 i = 51 ; i >= 0 ; --i){
        putchar('0' + ((fp.trans.frac&(1ULL<<(u64)i))>>(u64)i) );
    }
    putchar('\n');
    printf("%.3000lg = (-1)^%d + (1", fp.f, fp.trans.sgn);
    for(i64 i = 51 ; i >= 0 ; --i){
        if(((fp.trans.frac&(1ULL<<(u64)i))>>(u64)i)){
            printf(" + 2^-%lu", 52-i);
        }
    }
    printf(") * 2^(%u - 1023)", fp.trans.exp);
    putchar('\n');
    return 0;
}
