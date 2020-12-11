#include "basic.h"
#include "sort.h"
#include <stdio.h>

#ifndef DEBUG
#include "test.h"
#else
i32 array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
i32 array_size = sizeof(array) / sizeof(*array);
#endif
bool cmp(i32, i32);
int main() {
    puts("Before:");
    for (size_t i = 0; i < (size_t)array_size; ++i) {
        printf("%d ", array[i]);
    }
    putchar('\n');
    sort(array, array_size, cmp);
    puts("After:");
    for (size_t i = 0; i < (size_t)array_size; ++i) {
        printf("%d ", array[i]);
    }
    putchar('\n');
}

bool cmp(i32 a, i32 b){
    if( (a^b) & 1 ){ // a b -> odd even
        return (a&1);
    }
    else{ 
        if(a & 1){ //odd
            return a < b;
        }
        else{ //even
            return a > b;
        }
    }
}