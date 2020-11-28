#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
typedef int32_t i32;
typedef int64_t i64;
i32 w, h;
i32 tda(i32 x, i32 y){
	return x+y*w;
}
int main(){
	printf("Please enter the width : ");
	i32 err = scanf("%d", &w);
	if(err < 1 || w <= 0){
		if(~err) // Not EOF
			fprintf(stderr, "INVALID INPUT\n");
		exit(0);
	}
	printf("Please enter the height : ");
	err = scanf("%d", &h);
	if(err < 1 || h <= 0){
		if(~err) // Not EOF
			fprintf(stderr, "INVALID INPUT\n");
		exit(0);
	}
	i32 *a = calloc(w*h, sizeof(i32));
	i32 x, y;
	i32 total = a[tda(x=0,y=0)] = 1;
	while(total < w*h){
		while(x+1 < w && !a[tda(x+1, y)]) a[tda(++x, y)] = ++total;
		while(y+1 < h && !a[tda(x, y+1)]) a[tda(x, ++y)] = ++total;
		while(x-1 >= 0 && !a[tda(x-1, y)]) a[tda(--x, y)] = ++total;
		while(y-1 >= 0 && !a[tda(x, y-1)]) a[tda(x, --y)] = ++total;
	}
	for(i32 i = 0 ; i < h ; ++i){
		for(i32 j = 0 ; j < w ; ++j){
			printf("%*d ", (i32)ceil(log(w*h)/log(10))+1, a[tda(j, i)]);
		}
		putchar('\n');
	}
	free(a);
	return 0;
}