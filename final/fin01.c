#include <stdio.h>
#include "basic.h"
#include "geometry.h"

void print_func(i32 func[4]);

int main(){
    i32 func[4] = {0};
    printf("Please enter the plane: ");
    for(i32 i = 0 ; i < 4 ; ++i) scanf("%d", func + i);
    if(func[0] == func[1] && func[1] == func[2] && func[2] == 0){
        err("Invalid Plane Equation.");
    }
    printf("The plane is ");
    print_func(func);
    fp x, y, z;
    printf("Please enter the point: ");
    scanf("%lf%lf%lf", &x, &y, &z);
    mirror(&x, &y, &z, func[0], func[1], func[2], func[3]);
    printf("The mirror point is (%lf, %lf, %lf)\n", x, y, z);
    return 0;
}

void print_func(i32 func[4]){
    bool flag = true;
    static const char sym[] = {'x', 'y', 'z'};
    for(i32 i = 0 ; i < 4 ; ++i){
        if(func[i]){
            i32 opt = func[i];
            if(!flag){
                putchar("-+"[(func[i] > 0)]);
                putchar(' ');
                opt = abs(func[i]);
            }
            flag = false;
            printf("%d", opt);
            if(i < 3)
                putchar(sym[i]);
            putchar(' ');
        }
    }
    puts("= 0");
}