#include <stdio.h>
#include <ctype.h>

#include "basic.h"

int main(){
    printf("Please enter the hex string: ");
    bool fg = true;
    while(fg){
        char c = 0;
        char buf;
        byte *view = &c;
        for (int i = 0; i < 2; ++i) {
            if((buf = getchar()) == EOF) goto fail;
            if(buf == '\n') {
                fg = false;
                break;
            }
            if(!isxdigit(buf)){
                goto fail;
            }
            buf = tolower(buf);
            byte num = (isdigit(buf)?buf-'0':(buf-'a')+10);
            *view |= num << (4*(1-i));
        }
        if(!c) return 0;
        if(isprint(c)){
            putchar(c);
        }
        else{
            putchar('\n');
            err("Character is not printable");
        }
    }
    return 0;
    fail:
    putchar('\n');
    err("Input State Error!");
}