#include <stdio.h>
#include <stdint.h>
typedef int32_t dint;
int main(){
	dint num = 0;
	printf("Please enter an integer: ");
	scanf("%d", &num);
	printf("%d: %08X\n", num, num);
	return 0;
}