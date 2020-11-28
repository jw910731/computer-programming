#include <stdio.h>
#include "basic.h"
#include "roman.h"

int main(){
	i32 n;
	printf("Input a number (1~3000): ");
	scanf("%d", &n);
	if(n < 1 || n > 3000) err("INVALID INPUT.");
	roman(n);
	return 0;
}