#include "geometry.h"

#define SQ(x) ((x)*(x))

void mirror( double *_x, double *_y, double *_z, int32_t a, int32_t b, int32_t c, int32_t d ){
    const fp x = *_x, y = *_y, z = *_z;
    const fp coeff = ((a*x)+(b*y)+(c*z)+d) / (fp)(SQ(a)+SQ(b)+SQ(c));
    const fp dx = -a * coeff * 2, dy = -b * coeff * 2, dz = -c * coeff * 2;
    *_x = x+dx; *_y = y+dy; *_z = z+dz;
}