#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "guess_number.h"
#include "basic.h"
#include "color.h"
i32 cnt(i32 a);
void hint_message(const i32 A, const i32 B);


// Number Generator for game
i32 number_gen(i32 n){
	srand(time(NULL));
	i32 ret = 0, dupe_set = 0;
	while(n--){
		i32 tmp = rand()%10;
		while(dupe_set&(1<<tmp)){
			tmp = rand()%10;
		}
		ret = ret*10 + tmp;
		dupe_set = dupe_set | 1 << tmp;
	}
	return ret;
}

// response generator
i32 guess(i32 ans, i32 guess, i32 n){
	i32 Acnt = 0, Bcnt = 0, len = n;
	// Count A & B
	for(i32 i = 0, idiv = 1 ; i < len ; ++i, idiv*=10){
		for(i32 j = 0, jdiv = 1 ; j < len ; ++j, jdiv*=10){
			if((ans/idiv)%10 == (guess/jdiv)%10){
				if(i == j) ++Acnt;
				else ++Bcnt;
			}
		}
	}
	if(len > Acnt)
		hint_message(Acnt, Bcnt);
	else{
		printf(CLR_LGRN CLR_BOLD"Bingo! Congratulations."CLR_RST);
	}
	return len - Acnt;
}

void hint_message(const i32 A, const i32 B){
	if(A > 0)
		printf(CLR_24_FG("252","127","0") CLR_BOLD"%d"CLR_RST CLR_LRED" A "CLR_RST, A);
	if(B > 0)
		printf(CLR_24_FG("252","127","0") CLR_BOLD"%d"CLR_RST CLR_BL " B"CLR_RST, B);
	if(A <= 0 && B <= 0)
		printf(CLR_RED"\t:( You are stupid :(\n\t\t\tInvalid Guess"CLR_RST);
}

i32 cnt(i32 a){
	i32 ret = 0;
	while(a){
		++ret;
		a/=10;
	}
	return ret;
}

bool unique_ck(i32 n, i32 size){
	i32 dupe = 0;
	while(size--){
		if(dupe & (1<<(n%10))){
			return false;
		}
		dupe = dupe | (1<<(n%10));
		n /= 10;
	}
	if(n) return false;
	return true;
}
