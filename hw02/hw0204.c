#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
typedef int64_t i64;
typedef int32_t i32;
typedef struct{
	i32 d, m, y;
}Date;
const i64 monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; 
i32 countLeap(Date d) { 
    i32 years = d.y; 
    if(d.m <= 2) 
        years--;
    return years/4 - years/100 + years/400;
}
i64 dateDiff(Date dt1, Date dt2) { 
    i64 n1 = dt1.y*365 + dt1.d;  
    for (i32 i=0; i<dt1.m - 1; i++) 
        n1 += monthDays[i]; 
    n1 += countLeap(dt1); 
    i64 n2 = dt2.y*365 + dt2.d;
    for (int i=0; i<dt2.m - 1; i++) 
        n2 += monthDays[i]; 
    n2 += countLeap(dt2);
    return (n2 - n1);
}
inline bool isLeapYear(i32 y){return (!(y&3) && (y%25) )|| !(y&15);}
bool ck(Date d){
	return (d.m <= 12) && (d.m > 0) && (d.y > 0) && (d.d <= monthDays[d.m-1]+(i64)(bool)(isLeapYear(d.y) && d.m==2) && (d.d > 0));
}
int main(){
	Date d1, d2;
	printf("Date Format: YYYY/MM/DD\n");
	printf("Start Date: ");
	int err = scanf("%d / %d / %d", &d1.y, &d1.m, &d1.d);
	if(err < 3 || !ck(d1)){
		if(d1.y <= 0) {
			fprintf(stderr, "B.C is not supported!");
		}
		else{
			fprintf(stderr, "Invalid Input Recieved!");
		}
		fprintf(stderr, "Program will be terminated.\n");
		exit(1);
	}
	printf("End Date: ");
	err = scanf("%d / %d / %d", &d2.y, &d2.m, &d2.d);
	if(err < 3 || !ck(d2)){
		if(d2.y <= 0) {
			fprintf(stderr, "B.C is not supported!");
		}
		else{
			fprintf(stderr, "Invalid Input Recieved!");
		}
		fprintf(stderr, "Program will be terminated.\n");
		exit(1);
	}
	printf("Duration: %ld Day(s)\n", dateDiff(d1, d2));
	return 0;
}