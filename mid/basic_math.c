#include <math.h>
#include <stdbool.h>
#include "basic_math.h"
i32 dot(vec a, vec b){
	return (a.x*b.x) + (a.y*b.y);
}

i32 cross(vec a, vec b){
	return (a.x*b.y) - (a.y*b.x);
}

i32 lenSQ(vec a){
	return dot(a, a);
}

i32 len(vec a){
	return round(sqrt(lenSQ(a)));
}

vec add(vec a, vec b){
	return (vec){a.x+b.x, a.y+b.y};
}

vec sub(vec a, vec b){
	return (vec){a.x-b.x, a.y-b.y};
}

vec mul(i32 m, vec a){
	return (vec){m*a.x, m*a.y};
}

// get intersection of line
vec intersect(line u, line v){
	vec ac = sub(v.a, u.a), ad = sub(v.b, u.a);
	vec ab = getLineVec(u);
	vec x = mul((len(ad)*len(ab))/(len(ac)+len(ad)), ac);
	vec y = mul((len(ac)*len(ab))/(len(ac)+len(ad)), ad);
	return add(x, y);
}

// check two line is parallel
bool parallel(line u, line v){
	return cross(getLineVec(u), getLineVec(v)) == 0;
}