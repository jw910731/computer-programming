#include "basic.h"
#include <stdio.h>
#include <stdbool.h>
#include "cardutil.h"
#include "color.h"

#ifndef DEBUG
#include "test.h"
#else
#include "test_example.h"
#endif

static Card players[4][13];

void init(){
    bool warn_fg = false;
    static byte bucket[60] = {0};
    static const byte *target[4]= {player1, player2, player3, player4};
    for(i32 i = 0 ; i < 4 ; ++i){
        for(i32 j = 0 ; j < 13 ; ++j){
            if(target[i][j] > 52){
                err(CLR_RED"[Fatal] Invalid hand card input may lead the program run in unattended way."CLR_RST);
            }
            if(bucket[target[i][j]]){
                warn_fg = true;
            }
            // prevent input overflows the bucket.
            if(target[i][j] <= 52)
                ++bucket[target[i][j]];
            
            players[i][j] = create_card(target[i][j]);
        }
    }
    // give warning if 
    if(warn_fg){
        fprintf(stderr, CLR_LYLW"[Warning] Duplicated hand card or invalid hand card detected!\n"CLR_RST);
    }
}

i32 rank_cmp( const Card *a, const Card *b );
i32 poker_cmp( const Card *a, const Card *b );
i32 suite_cmp( const Card *a, const Card *b );

int main(){
    init();
    printf( "Before:\n" );
    for(i32 i = 0 ; i < 4 ; ++i){
        print_card(players[i]);
    }
    static i32 ( * cmps[3])( const Card *a, const Card *b ) = {rank_cmp, poker_cmp, suite_cmp};
    for(i32 ix = 0 ; ix < 3 ; ++ix){
        printf("Type %02d: \n", ix+1);
        for(i32 i = 0 ; i < 4 ; ++i){
            sort_card(players[i], cmps[ix]);
            print_card(players[i]);
        }
    }
    return 0;
}

i32 rank_cmp( const Card *a, const Card *b ){
    i32 _a = (a->rank + 12)%13, _b = (b->rank + 12)%13;
    return _a > _b;
}
i32 poker_cmp( const Card *a, const Card *b ){
    i32 _a = (a->rank + 11)%13, _b = (b->rank + 11)%13;
    return _a > _b;
}
i32 suite_cmp( const Card *a, const Card *b ){
    return a->s > b->s;
}