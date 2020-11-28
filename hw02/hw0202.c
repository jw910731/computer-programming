#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
typedef int64_t i64;
typedef int32_t i32;
typedef struct{
    i64 x, y;
}Vec;
typedef struct{
	Vec pt1, pt2;
}Line;
typedef struct{
	double x, y;
}fVec;
inline Vec sub(Vec a, Vec b){
	return (Vec){a.x - b.x, a.y - b.y};
}
inline Vec lineVec(Line l){
	return sub(l.pt2, l.pt1);
}
inline i64 cross(Vec a, Vec b){
	return (a.x*b.y) - (a.y*b.x);
}
inline Vec mul(Vec v, i64 m){
	return (Vec){v.x*m, v.y*m};
}
bool intersectChecker(Line a, Line b){
	i64 areaA = cross(lineVec(a), sub(b.pt1, a.pt1));
	i64 areaB = cross(lineVec(a), sub(b.pt2, a.pt1));
	return (areaA - areaB) != 0;
}
fVec intersect(Line a, Line b){
	i64 areaA = cross(lineVec(a), sub(b.pt1, a.pt1));
	i64 areaB = cross(lineVec(a), sub(b.pt2, a.pt1));
	Vec tmp = sub(mul(b.pt2, areaA), mul(b.pt1, areaB));
	double areaDiff = areaA - areaB;
	return (fVec){tmp.x/areaDiff, tmp.y/areaDiff};
}
const char *msg[] = {"first", "second"};
int main(){
    Line line[2];
    for(int i = 0 ; i < 2 ; ++i){
    	int ret;
    	Vec tmp[2];	
        printf("Please enter the %s line in format '(x1, y1),(x2, y2)': ", msg[i]);
        ret = scanf(" ( %ld , %ld ) , ( %ld , %ld )", &tmp[0].x, &tmp[0].y, &tmp[1].x, &tmp[1].y);
        if(ret == EOF) return 0;
        if(ret < 4){
            fprintf(stderr, "Invalid Input Recieved! Program will be terminated\n");
            exit(1);
        }
        line[i] = (Line){tmp[0], tmp[1]};
    }
    printf("Intersection: ");
    if(intersectChecker(line[0], line[1])){
    	fVec ans = intersect(line[0], line[1]);
    	printf("(%.6lf, %.6lf)\n", ans.x, ans.y);
    }
    else{
    	printf("Does not exist.\n");
    }
    return 0;
}