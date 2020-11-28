#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
typedef int64_t sblt;
typedef int32_t sbt;
int main(){
    sblt a, b,c;
    printf("Please enter a quadratic polynomial in format \'a b c\': ");
    int err = scanf("%ld%ld%ld", &a, &b, &c);
    if(err == EOF) return 0;
    if(err < 3){
        fprintf(stderr, "Invalid Input Recieved. Program will be terminated.\n");
        exit(1);
    }
    const sblt deter1 = b*b, deter2 = 4*a*c;
    if(deter1 > deter2)
        printf("Two distinct real roots.\n");
    else if (deter1 < deter2)
        printf("No real roots.\n");
    else
        printf("One real root.\n");
    return 0;
}