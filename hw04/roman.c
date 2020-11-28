#include <stdio.h>
#include "basic.h"

void p_mul_char(const char *ipt,int time){
    for(int i=0;i<time;i++){
        printf("%s", ipt);
    }
}

void roman(int num){
    int cnt=0;
    if(num>=1000){
        p_mul_char("M",num/1000);
        num %=1000;
        cnt++;
    }
    if(num>=900){
        p_mul_char("CM",num/900);
        num %=900;
        cnt++;
    }
    if(num>=500){
        p_mul_char("D",num/500);
        num %=500;
        cnt++;
    }
    if(num>=400){
        p_mul_char("CD",num/400);
        num %=400;
        cnt++;
    }
    if(num>=100){
        p_mul_char("C",num/100);
        num %=100;
        cnt++;
    }
    if(num>=90){
        p_mul_char("XC",num/90);
        num %=90;
        cnt++;
    }
    if(num>=50){
        p_mul_char("L",num/50);
        num %=50;
        cnt++;
    }
    if(num>=40){
        p_mul_char("XL",num/40);
        num %=40;
        cnt++;
    }
    if(num>=10){
        p_mul_char("X",num/10);
        num %=10;
        cnt++;
    }
    if(num>=9){
        p_mul_char("IX",num/9);
        num %=9;
        cnt++;
    }
    if(num>=5){
        p_mul_char("V",num/5);
        num %=5;
        cnt++;
    }
    if(num>=4){
        p_mul_char("IV",num/4);
        num %=4;
        cnt++;
    }
    if(num>=1){
        p_mul_char("I",num/1);
        cnt++;
    }
}