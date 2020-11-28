#ifndef BASIC_MATH_H
#define BASIC_MATH_H
#include <stdbool.h>
#include "basic.h"
typedef double fp;

typedef struct{
	i32 x, y;
}vec;

typedef struct{
	fp x, y;
}fvec;

typedef struct{
	vec a, b;
}line;

vec add(vec a, vec b);
vec mul(i32 m, vec a);
vec sub(vec a, vec b);

// get line side vector
vec getLineVec(line a);

// get intersection of line
vec intersect(line u, line v);

// check two line is parallel
bool parallel(line u, line v);

i32 dot(vec a, vec b);
i32 cross(vec a, vec b);
// return it's lenth^2
i32 lenSQ(vec a);
#endif