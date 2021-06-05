#include "vec.h"

#include <math.h>

struct vec_t add(const struct vec_t a, const struct vec_t b) {
    return (struct vec_t){.x = a.x + b.x, .y = a.y + b.y};
}

struct vec_t sub(const struct vec_t a, const struct vec_t b) {
    return (struct vec_t){.x = a.x - b.x, .y = a.y - b.y};
}

struct vec_t rotate(const struct vec_t a, const fp deg) {
    const fp cs = cos(deg), sn = sin(deg);
    return (struct vec_t){.x = a.x * cs - a.y * sn, .y = a.x * sn + a.y * cs};
}

fp dot(const struct vec_t a, const struct vec_t b) {
    return a.x * b.x + a.y * b.y;
}
fp cross(const struct vec_t a, const struct vec_t b) {
    return a.x * b.y - a.y * b.x;
}