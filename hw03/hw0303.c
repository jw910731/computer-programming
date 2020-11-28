#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
typedef int32_t i32;
typedef i32(*state)(i32);
i32 start(i32 n){
	if(n == 10 || n == 35) return 1;
	else if(n == 11) return 3;
	else if(n == 20 || n == 78) return 5;
	else return 0;
}
i32 s1(i32 n){
	if(n == 19) return 2;
	else if(n == 12 || n == 36) return 6;
	else return 1;
}
i32 s2(i32 n){
	if(n == 43) return 2;
	else if(n == 99) return 7;
	else return 0;
}
i32 s3(i32 n){return 4;}
i32 s4(i32 n){return 6;}
i32 s5(i32 n){
	if(n == 1) return 4;
	else if(n == 2) return 6;
	else return 0;
}
i32 s6(i32 n){
	if(n == 108) return 7;
	else return 5;
}
const state fun_list[8] = {start, s1, s2, s3, s4, s5, s6, NULL};
const char* name_list[8] = {"Start", "S1", "S2", "S3", "S4", "S5", "S6", "Final"};
int main(){
	i32 now = 0, tmp;
	while(true){
		printf("%s\n", name_list[now]);
		if(fun_list[now] == NULL) break;
		printf("Please enter an integer: ");
		if(scanf("%d", &tmp) < 1){fprintf(stderr, "INVALID INPUT\n");exit(1);}
		now = fun_list[now](tmp);
	}
	return 0;
}