#pragma once
#include <stdint.h>
#include <stdlib.h>
typedef int32_t i32;

typedef struct {
    size_t w, h;
    i32 *data;
} Matrix;

Matrix *create_mat(size_t w, size_t h);
size_t at(Matrix *mat, size_t x, size_t y);
Matrix *mul(Matrix *a, Matrix *b);
void free_mat(Matrix **mat);
void print_mat(Matrix *mat);