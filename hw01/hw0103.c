#include <stdio.h>

int main(){
	double g, t;
	printf("Please enter the acceleration due to gravity: ");
	scanf("%lf", &g);
	printf("Please enter the time (s): ");
	scanf("%lf", &t);
	printf("Final velocity: %.350lg m/s\n", g*t);
	printf("The altitude: %.350lgm\n", g*t*t/2.0);
	return 0;
}