#pragma once

#include "basic.h"

struct vec_t {
    fp x, y;
};

struct vec_t add(const struct vec_t a, const struct vec_t b);
struct vec_t sub(const struct vec_t a, const struct vec_t b);
fp dot(const struct vec_t a, const struct vec_t b);
fp cross(const struct vec_t a, const struct vec_t b);
/*
 * Rotate input vector @param a @param deg degree in radian
 * @param a vector to be rotated
 * @param deg rotate angle in radian
 */
struct vec_t rotate(const struct vec_t a, const fp deg);
