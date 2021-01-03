#include "rotate.h"
#include <math.h>

void rotate( fp *x, fp *y, fp theta ){
    fp _x = *x, _y = *y;
    fp s = sin(-theta), c = cos(-theta);
    *x = _x*c - _y*s;
    *y = _y*c + _x*s;
}