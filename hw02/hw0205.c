#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
typedef int64_t i64;
typedef int32_t i32;

typedef enum{
	spade = 0, heart, diamond, club
}Suit;
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
typedef struct {
	Suit s;
	i32 rank;
}Card;
inline Card convert(i32 num){
	return (Card){(num-1)/13, (num-1)%13};
}
int cardCmp(const void *_a, const void *_b){
	Card a = *(Card*)_a, b = *(Card*)_b;
	if(a.rank == b.rank){
		return suitCmp(a.s, b.s);
	}
	else{
		return intCmp(a.rank, b.rank);
	}
}
inline void cardSort(Card *begin, Card *end){
	qsort(begin, end-begin, sizeof(begin[0]), cardCmp);
}
i32 kinds(Card deck[5]){
	i32 bucket[13];
	memset(bucket, 0x00, sizeof(bucket));
	for(i32 i = 0 ; i < 5 ; ++i){
		++bucket[deck[i].rank];
	}
	i32 mx = 0, submx = 0;
	for(i32 i = 0 ; i < 13 ; ++i){
		if(bucket[i] >= mx){
			submx = mx;
			mx = bucket[i];
		}
	}
	if(mx <= 1) return 0;
	else if(mx == 2){
		if(submx == mx){
			return 2;
		}
		else{
			return 1;
		}
	}
	else{
		return mx;
	}
}
// same suit
bool flush(Card deck[5]){
	Suit s = deck[0].s;
	for(i32 i = 1 ; i < 5 ; ++i){
		if(s != deck[i].s) return false;
	}
	return true;
}
bool straight(Card deck[5]){
	i32 r = deck[0].rank;
	for(i32 i = 1 ; i < 5 ; ++i){
		if(deck[i].rank != r+i) return false;
	}
	return true;
}
int main(){
	Card deck[5];
	printf("Please enter 5 cards seperated by spaces: ");
	for(i32 i = 0 ; i < 5 ; ++i){
		int err;
		i32 tmp;
		err = scanf("%d", &tmp);
		if(err == EOF) exit(0);
		if(err < 1 || tmp < 1 || tmp > 52){
			fprintf(stderr, "Invalid Input Recieved! Program will be terminated.\n");
			exit(1);
		}
		deck[i] = convert(tmp);
	}
	cardSort(deck, deck+5);
	//repeat chekcer
	for(i32 i = 0 ; i < 5-1 ; ++i){
		if(cardCmp(&deck[i], &deck[i+1]) == 0){
			fprintf(stderr, "Repeated card in input! Program will be terminated.\n");
			exit(1);
		}
	}
	/*for(int i = 0 ;  i < 5 ; ++i)
		printf("%d, %d\n", deck[i].s, deck[i].rank);*/
	i32 kind = kinds(deck);
	bool s = straight(deck), f = flush(deck);
	if(s && f)
		printf("Straight Flush\n");
	else if(kind == 4)
		printf("Four of a kind\n");
	else if(f)
		printf("Flush\n");
	else if(s)
		printf("Straight\n");
	else if(kind == 3)
		printf("Three of a kind\n");
	else if(kind == 2)
		printf("Two pair\n");
	else if(kind == 1)
		printf("One pair\n");
	else
		printf("High card\n");
	return 0;
}