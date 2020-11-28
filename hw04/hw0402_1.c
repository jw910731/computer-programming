#include <stdio.h>
#include "basic.h"
#include "hanoi.h"

int main(){
	i32 n;
	printf("Please enter the disk number (2-20): ");
	scanf("%d", &n);
	if(n < 2 || n > 20) err("INPUT INVALID.");
	hanoi(n, 1, 3, 2);
	return 0;
}