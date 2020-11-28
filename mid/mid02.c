#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "basic.h"
#include "basic_math.h"

int main(){
	/*line lines[4];
	for(i32 i = 0 ; i < 4 ; ++i){
		printf("Please enter P%d, P%d (x%d,y%d),(x%d,y%d)\n", i*2+1, i*2+2, i*2+1, i*2+1, i*2+2, i*2+2);
		scanf(" (%d,%d) , (%d,%d)", &lines[i].a.x, &lines[i].a.y, &lines[i].b.x, &lines[i].b.y);
	}
	if(parallel(lines[0], lines[2]) || parallel(lines[1], lines[3])){
		err("This is not a parallelogram.");
	}
	vec poly[4];
	for(i32 i = 0 ; i < 4 ; ++i){
		poly[i] = intersect(lines[i], lines[(i+1)%4]);
	}*/
	line a, b;
	scanf("%d%d%d%d", &a.a.x, &a.a.y, &a.b.x, &a.b.y);
	scanf("%d%d%d%d", &b.a.x, &b.a.y, &b.b.x, &b.b.y);
	vec x = intersect(a, b);
	printf("(%d, %d)\n", x.x, x.y);
	return 0;
}