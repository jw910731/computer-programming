//
// Created by jw910731 on 3/20/21.
//

#include "basic.h"
#include <stdio.h>

union soviet{
    byte b;
    struct{
        uint8_t b8 : 1;
        uint8_t b7 : 1;
        uint8_t b6 : 1;
        uint8_t b5 : 1;
        uint8_t b4 : 1;
        uint8_t b3 : 1;
        uint8_t b2 : 1;
        uint8_t b1 : 1;
    }trans;
};

#define BIT(b, s) do{ \
for(i64 i = s-1 ; i >= 0 ; --i)\
    putchar('0' + (((b)&(1ULL<<(u64)i))>>(u64)i) );\
}while(0);             \

int main(){
    union soviet u;
    int ret = 1;
    do {
        if(ret != 1){
            puts("Input Invalid.");
        }
        printf("Please enter a byte (0-255): ");
        i32 tmp;
        ret = scanf("%d", &tmp);
        while(getchar() != '\n');
        if(tmp < 0 || tmp > 255) ret = 2;
        else u.b = tmp;
    }while(ret != 1);
    i32 tmp = 0;
    do{
        printf("Data: %u ", u.b);
        BIT(u.b, 8);
        putchar('\n');
        ret = 1;
        do{
            if(ret != 1){
                puts("Input Invalid.");
            }
            printf("Flip bit (1-8, 0: exit): ");
            ret = scanf("%d", &tmp);
            while(getchar() != '\n');
            if(tmp < 0 || tmp > 8) ret = 2;
        }while(ret != 1);
        switch (tmp) {
            case 1:
                u.trans.b1 ^= 1;
                break;
            case 2:
                u.trans.b2 ^= 1;
                break;
            case 3:
                u.trans.b3 ^= 1;
                break;
            case 4:
                u.trans.b4 ^= 1;
                break;
            case 5:
                u.trans.b5 ^= 1;
                break;
            case 6:
                u.trans.b6 ^= 1;
                break;
            case 7:
                u.trans.b7 ^= 1;
                break;
            case 8:
                u.trans.b8 ^= 1;
                break;
            default:
                break;
        }
    }while(tmp);
    puts("Bye");
    return 0;
}