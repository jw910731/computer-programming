#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(){
	uint32_t n;
	int err;
	printf("Please enter a natural number :");
	err = scanf("%u", &n);
	if(err < 1 || (n&1<<31)){
		if(~err)
			fprintf(stderr, "INVALID INPUT\n");
		exit(1);
	}
	while(n > 0){
		printf("%d", n%10);
		n/=10;
	}
	return 0;
}