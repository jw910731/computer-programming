#include "taylor.h"
#include "basic.h"

fp euler_exp(i64 exp, i64 k){
	i64 fact = 1, pow = 1;
	fp sum = 1;
	for(i64 i = 1 ; i <= k ; ++i){
		fact *= i; pow *= exp;
		sum += ((fp)pow/(fp)fact);
	}
	return sum;
}