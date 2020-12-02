#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
typedef int32_t i32;
int main(){
	Matrix *mat[2];
	for(size_t ix = 0 ; ix < 2 ; ++ix){
		size_t w, h;
		printf("Input Matrix width and height : ");
		scanf("%lu%lu", &w, &h);
		mat[ix] = create_mat(w, h);
		printf("Now Input the Matrix :\n");
		for(size_t i = 0 ; i < w ; ++i){
			for(size_t j = 0 ; j < h ; ++j){
				scanf("%d", &(mat[ix]->data[at(mat[ix], i, j)]));
			}
		}
	}
	Matrix *ans = mul(mat[1], mat[0]);
	if(ans != NULL){
		printf("Result Matrix : \n");
		print_mat(ans);
	}
	else{
		printf("These two matrix is not multipliable.\n");
	}
	// free resource
	free_mat(mat[0]);
	free_mat(mat[1]);
	if(ans != NULL)
		free_mat(ans);
	return 0;
}