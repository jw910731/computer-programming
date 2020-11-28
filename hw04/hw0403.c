#include <stdio.h>
#include <math.h>
#include "basic.h"
#include "taylor.h"

int main(){
	i32 k;
	printf("k-th order Taylor polynomial for e\nPlease enter k: ");
	scanf("%d", &k);
	printf("%.1000lg\n", euler_exp(1, k));
	return 0;
}