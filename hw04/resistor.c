#include "resistor.h"

fp parallel(fp a, fp b){
	return (a*b)/(a+b);
}

fp equal_resistor(i32 g_resist, i32 n){
	if(n == 1) return (fp) g_resist*2;
	return parallel(equal_resistor(g_resist, n-1), g_resist) + g_resist;
}
