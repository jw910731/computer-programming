#include "matrix.h"
#include <stdio.h>
// constructor
Matrix *create_mat(size_t w, size_t h) {
    Matrix *ret = calloc(1, sizeof(Matrix));
    ret->data = calloc(w * h, sizeof(i32));
    ret->w = w;
    ret->h = h;
    return ret;
}
size_t at(Matrix *mat, size_t x, size_t y) { return x + (y * mat->w); }
Matrix *mul(Matrix *a, Matrix *b) {
    if (a->w != b->h)
        return NULL;
    Matrix *ret = create_mat(b->w, a->h);
    for (size_t i = 0; i < a->h; ++i) {     // y idx
        for (size_t j = 0; j < b->w; ++j) { // x idx
            // mul a->row and b->col
            for (size_t k = 0; k < b->h; ++k) {
                ret->data[at(ret, i, j)] +=
                    b->data[at(b, j, k)] * a->data[at(a, k, i)];
            }
        }
    }
    return ret;
}
// destructor
void free_mat(Matrix **mat) {
    free((*mat)->data);
    free(*mat);
    *mat = NULL;
}
// print matrix
void print_mat(Matrix *mat) {
    for (size_t i = 0; i < mat->h; ++i) {
        for (size_t j = 0; j < mat->w; ++j) {
            printf("%3d ", mat->data[at(mat, j, i)]);
        }
        putchar('\n');
    }
}