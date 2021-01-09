#include "cardutil.h"
#include <stdlib.h>
#include <stdio.h>

inline int suitCmp(Suit a, Suit b){
	if(a > b) return 1;
	else if(a < b) return -1;
	else return 0;
}
inline int intCmp(i32 a, i32 b){
	if(a > b) return 1;
	else if(a < b) return -1;
	else return 0;
}

Card create_card(byte num){
	return (Card){(num-1)/13, (num-1)%13};
}

byte cardtoi(Card c){
    return (c.s*13)+c.rank+1;
}

// Decorator
static i32 (*_cmp)(const Card *a, const Card *b ) = NULL;
static int cmp_wrapper(const void *a, const void *b){
    if(!_cmp) err("[Fatal] Something went wrong. Sorry >.<");
    return _cmp((const Card*)a, (const Card*)b);
}

i32 sort_card( Card player[13], i32 ( * cmp)( const Card *a, const Card *b ) ){
    _cmp = cmp;
    qsort(player, 13, sizeof(Card), cmp_wrapper);
    _cmp = NULL;
    return 0;
}

void print_card( const Card player[13] ){
    for(i32 i = 0 ; i < 13 ; ++i){
        printf("%2d ", cardtoi(player[i]));
    }
    putchar('\n');
}