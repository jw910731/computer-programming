#pragma once

#include "basic.h"

typedef int32_t i32;
typedef enum{
	spade = 0, heart, diamond, club
}Suit;

typedef struct {
	Suit s;
	i32 rank;
}Card;

Card create_card(byte num);
byte cardtoi(Card c);
void print_card( const Card player[13] );
i32 sort_card( Card player[13], i32 ( * cmp)( const Card *a, const Card *b ) );