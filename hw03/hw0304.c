#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#define LIM 16
#define SQ(x) ((x)*(x))
#define ABS(x) (((x)<0)?(-(x)):(x))
typedef double fp;
typedef fp(*mFunc)(fp);
fp integralHelper(fp);
fp riemanIntegral(fp, mFunc);
fp func(fp);
void err(const char*);
void printPoly();
int32_t coeff[3], t, s;
int main(){
	printf("Please enter a quadratic polynomial (ax^2+bx+c): ");
	scanf(" %d , %d , %d", coeff+2, coeff+1, coeff);
	if(coeff[2] == 0){ // prevent non quadratic polynominal
		err("Input is not a quadratic polynomial.");
	}
	printf("The polynomial is ");
	printPoly();
	printf("Please enter the interval [s,t]: ");
	scanf("%d , %d", &s, &t);
	if(t < s){ // prevent inverse interval
		err("This Program does not support inversed interval.");
	}
	printf("The integral: %lf\n", integralHelper(t)-integralHelper(s));
	for(int32_t i = 2 ; i <= (1<<LIM) ; i = i<<1){
		printf("The Riemann sum of n=%d: %lf\n", i, riemanIntegral(i, func));
	}
	return 0;
}

void printPoly(){ // print polynominal
	// x^2 must exist
	if(coeff[2] != 1) printf("%d", coeff[2]);
	printf("x^2 ");
	for(int i = 1 ; i >= 0 ; --i){
		if(coeff[i] != 0){ // non zero -> output
			if(coeff[i] > 0) putchar('+');
			else putchar('-');
			printf(" %d", ABS(coeff[i]));
			if(i >= 1) putchar('x');
			putchar(' ');
		}
	}
	putchar('\n');
}

inline fp integralHelper(fp x){
	return (SQ(x)*x*coeff[2]/3)+(SQ(x)*coeff[1]/2)+(coeff[0]*x);
}


void err(const char *s){ // error handler
	fprintf(stderr, "%s\nFatal error occured and thus program is going to terminate.\n", s);
	exit(1);
}

fp func(fp x){ // polynominal function
	return (SQ(x)*(fp)coeff[2])+(x*(fp)coeff[1])+(fp)coeff[0];
}

fp riemanIntegral(fp n, mFunc f){ // integral helper
	fp sum = 0;
	for(int32_t i =  0 ; i < n ; ++i){
		fp progress = ((fp)(t-s)*(fp)i/n);
		sum += f(s+progress);
	}
	return sum*((fp)(t-s)/n);
}