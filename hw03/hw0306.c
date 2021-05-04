#include <stdio.h>
#include <stdint.h>

/*
 * This implementation of max only work when (a-b) is in the int32_t representation range
 * If exceed, the behaviour is undefined.
 */
void max( int32_t a, int32_t b ){
    printf("%d\n", a - ((a-b) & (a-b)>>31));
    return;
}

int main(){
    max(10, 20);
    return 0;
}
