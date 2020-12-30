#include "basic.h"
#include <stdio.h>
#include "color.h"
int main(){
    i64 n;
    uint8_t *arr;
    printf("Please input an unsigned (positive) integer: ");
    scanf("%lld", &n);
    arr = &n;
    while(1){
        i32 pos = -1, val = -1;
        printf("The integer: %lld\n", n);
        for(i32 i = 0 ; i < 8 ; ++i){
            printf("(%d) 0x%02X ", i+1, arr[i]);
        }
        putchar('\n');
        bool fg = false;
        while(pos < 0 || pos > 8){
            if(fg){
                puts(CLR_LRED"Invalid Input."CLR_RST);
            }
            fg = true;
            printf("Please enter the position (1-8, 0: End): ");
            scanf("%d", &pos);
            // stdin flusher
            for(char c; (c=getchar()) != '\n' && c != EOF ;);
        }
        if(!pos){
            break;
        }
        --pos; // offset pos var to 0-index
        fg = false;
        while(val < 0 || val > 255){
            if(fg){
                puts(CLR_LRED"Invalid Input."CLR_RST);
            }
            printf("Please enter the new value (0-255): ");
            scanf("%d", &val);
            // stdin flusher
            for(char c; (c=getchar()) != '\n' && c != EOF ;);
        }
        arr[pos] = val;
    }
    return 0;
}