#include "basic.h"
#include "sort.h"
#include <stdio.h>

#ifndef DEBUG
	#include "test.h"
#else
	i32 array[10] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
	i32 array_size = sizeof(array)/sizeof(*array);
#endif

int main(){
	puts("Before:");
	for(size_t i = 0 ; i < array_size ; ++i){
		printf("%d ", array[i]);
	}
	putchar('\n');
	sort(array, array_size);
	puts("After:");
	for(size_t i = 0 ; i < array_size ; ++i){
		printf("%d ", array[i]);
	}
	putchar('\n');
}