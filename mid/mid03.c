#include <stdio.h>
#include "basic.h"

int main(){
	int a, b;
	printf("Please enter the Gashapon number : ");
	scanf("%d", &a);
	if(a < 0) err("INVALID INPUT");
	printf("lease enter the exchange number : ");
	scanf("%d", &b);
	if(b <= 1) err ("INVALID INPUT");
	i32 cnt = a;
	while(a >= b){
		i32 tmp = a / b;
		a= a%b + tmp;
		cnt += tmp;
	}
	printf("You can totally open %d Gashapon balls.\n", cnt);
	return 0;
}