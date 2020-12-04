#include "matrix.h"
#include "basic.h"
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

i32 det(Matrix *a) {
    if (a == NULL)
        return 0;
    if (a->w != a->h)
        return 0;
    if (a->w <= 1)
        return a->data[0];
    size_t s = a->w; // w == h of Matrix
    i32 sum = 0;
    if (s == 2) {
        return (a->data[at(a, 0, 0)] * a->data[at(a, 1, 1)]) -
               (a->data[at(a, 0, 1)] * a->data[at(a, 1, 0)]);
    }
    for (size_t idx = 0; idx < s; ++idx) {
        Matrix *tmp = create_mat(s - 1, s - 1);
        // copy
        for (size_t i = 0, iflag = false; i < s; ++i) { // x idx
            for (size_t j = 1; j < s; ++j) {            // y idx
                if (i == idx) {
                    iflag = true;
                    continue;
                }
                tmp->data[at(tmp, i - iflag, j - 1)] = a->data[at(a, i, j)];
            }
        }
        sum += a->data[at(a, idx, 0)] * det(tmp) * ((idx % 2) ? 1 : -1);
        free_mat(&tmp);
    }
    return sum;
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