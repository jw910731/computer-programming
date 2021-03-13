#include <stdio.h>
#include <string.h>

#include "basic.h"
#include "color.h"
// extra space for newline and null terminator
char in_buf[1030], target[130], replace[130];
int main(){
    puts("Please enter the original text:");
    fgets_n(in_buf, 1025, stdin);
    puts("Keyword:");
    fgets_n(target, 130, stdin);
    puts("New word:");
    fgets_n(replace, 130, stdin);
    putchar('\n');
    puts("Before:");
    i32 target_siz = strlen(target);
    // occurrence of target
    char *occ = strstr(in_buf, target);
    for(char *it = in_buf; *it ; ++it){
        if(it != occ){ // not match
            putchar(*it);
        }
        else{
            fputs(CLR_LBL, stdout);
            fputs(target, stdout);
            fputs(CLR_RST, stdout);
            it += target_siz - 1;
            occ = strstr(it, target);
        }
    }
    puts("\nAfter:");
    occ = strstr(in_buf, target);
    for(char *it = in_buf; *it ; ++it){
        if(it != occ){ // not match
            putchar(*it);
        }
        else{
            fputs(CLR_LRED, stdout);
            fputs(replace, stdout);
            fputs(CLR_RST, stdout);
            it += target_siz - 1;
            occ = strstr(it, target);
        }
    }
    putchar('\n');
    return 0;
}