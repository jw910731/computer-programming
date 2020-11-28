#include <stdio.h>
#include <stdbool.h>
#include "basic.h"

bool ck(i32 a){
	i32 prev = 11;
	while(a>0){
		if(prev > a) return false;
		a/=10;
	}
	return true;
}

i32 flip(i32 a){	
	i32 i = 0;
	while(a>0){
		i = i*10 + a%10;
		a/=10;
	}
	return i;	
}

int main(){
	i32 a, b;
	printf("Please enter the 1st integer: ");
	scanf("%d", &a);
	printf("Please enter the 2nd integer :");
	scanf("%d", &b);
	if(ck(a) || ck(b)) err("INPUT INVALID.");
	i64 ans = 0;
	a = flip(a);
	b = flip(b);
	while(a > 0 && b > 0){
		if(a%10 < b%10){
			ans = ans*10 + a%10;
			a/=10;
		}
		else{
			ans = ans*10 + b%10;
			b/=10;
		}
	}
	while(a > 0){
		ans = ans*10 + a%10;
		a/=10;
	}
	while(b > 0){
		ans = ans*10 + b%10;
		b/=10;
	}
	printf("Result: %ld\n", ans);
	return 0;
}