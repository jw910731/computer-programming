#include <stdio.h>
#include "basic.h"
#include "resistor.h"

int main(){
	i32 r, n;
	printf("Please enter the resistance (1-100): ");
	scanf("%d", &r);
	if(r < 1 || r > 100) err("Invalid Input");
	printf("Please enter n (1-100): ");
	scanf("%d", &n);
	if(n < 1 || n > 100) err("Invalid Input");
	printf("Ans: %lf\n", equal_resistor(r, n));
}	