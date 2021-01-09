#include "geometry.h"
#include <math.h>
#include <stdlib.h>

#define SQ(x) ((x)*(x))

void rotate( fp *x, fp *y, fp theta ){
    fp _x = *x, _y = *y;
    fp s = sin(-theta), c = cos(-theta);
    *x = _x*c - _y*s;
    *y = _y*c + _x*s;
}

void project( fp *_x, fp *_y, fp *_z, const i32 a, const i32 b, const i32 c, const i32 d ){
    const fp x = *_x, y = *_y, z = *_z;
    const fp coeff = ((a*x)+(b*y)+(c*z)+d) / (fp)(SQ(a)+SQ(b)+SQ(c));
    const fp dx = -a * coeff, dy = -b * coeff, dz = -c * coeff;
    *_x = x+dx; *_y = y+dy; *_z = z+dz;
}