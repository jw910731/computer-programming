#include "sort.h"
#include <string.h>
// implemented by merge sort
void sort(i32 *a, size_t s){
	if(s <= 1) return;
	size_t lLen = s/2, rLen = s-lLen;
	// generate tmp array
	int *tmp = calloc(s, sizeof(i32));
	sort(a, lLen); // sort l side
	sort(a+lLen, rLen); // sort r side
	// merge
	int *l = a, *r = a+lLen;
	size_t li = 0, ri = 0, pivot = 0;
	while(li < lLen && ri < rLen){
		if(l[li] < r[ri]){
			tmp[pivot++] = l[li++];
		}
		else{
			tmp[pivot++] = r[ri++];
		}
	}
	while(li < lLen) tmp[pivot++] = l[li++];
	while(ri < rLen) tmp[pivot++] = r[ri++];
	memcpy(a, tmp, s*sizeof(i32));
	free(tmp);
}