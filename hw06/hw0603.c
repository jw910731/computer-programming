#include "basic.h"
#include "geometry.h"
#include <stdio.h>
#include <math.h>

int main(){
    const fp trans = asin(1) / 90.0;
    fp x, y;
    printf("Please enter a point: ");
    scanf("%lf %lf", &x, &y);
    fp angle;
    printf("Please enter theta (0-360): ");
    scanf("%lf", &angle);
    //debug("%lf %lf\n", asin(1) , tra)
    if(angle < 0 || angle > 360) err("Invalid Angle.");
    rotate(&x, &y,  angle * trans);
    printf("%lf %lf\n", x, y);
    return 0;
}