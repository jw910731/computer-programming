#include <stdio.h>
#include "basic.h"
#include "geometry.h"
#include <stdlib.h>

void print_eq(i32 eq[4]);

int main(){
    i32 eq[4] = {0};
    printf("Please enter the plane: ");
    for(i32 i = 0 ; i < 4 ; ++i) scanf("%d", eq + i);
    if(eq[0] == eq[1] && eq[1] == eq[2] && eq[2] == 0){
        err("Invalid Plane Equation.");
    }
    printf("The plane is ");
    print_eq(eq);
    fp x, y, z;
    printf("Please enter the point: ");
    scanf("%lf%lf%lf", &x, &y, &z);
    project(&x, &y, &z, eq[0], eq[1], eq[2], eq[3]);
    printf("The projection is (%lf, %lf, %lf)\n", x, y, z);
    return 0;
}

void print_eq(i32 eq[4]){
    bool fst_flag = true;
    static const char sym[4] = {'x', 'y', 'z', ' '}; // shouldn't acess last element
    for(i32 i = 0 ; i < 4 ; ++i){
        if(eq[i]){
            i32 opt = eq[i];
            if(!fst_flag){
                putchar("-+"[(eq[i] > 0)]);
                putchar(' ');
                opt = abs(eq[i]);
            }
            fst_flag = false;
            printf("%d", opt);
            if(i < 3)
                putchar(sym[i]);
            putchar(' ');
        }
    }
    puts("= 0");
}