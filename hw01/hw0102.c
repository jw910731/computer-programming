#include <stdio.h>
#include <stdint.h>
typedef int32_t dint;
int main(){
	dint prod = 1;
	char c = 0;
	printf("Please enter a 5-digits integer: ");
	for(dint i = 0 ; i < 5 ; ++i){
		scanf("%c", &c);
		prod *= (c-'0');
	}
	printf("Result: %d\n", prod);
	return 0;
}