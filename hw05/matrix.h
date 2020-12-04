#pragma once
#include "basic.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t w, h;
    i32 *data;
} Matrix;

Matrix *create_mat(size_t w, size_t h);
size_t at(Matrix *, size_t x, size_t y);
Matrix *mul(Matrix *, Matrix *);
i32 det(Matrix *);
void free_mat(Matrix **);
void print_mat(Matrix *);