#include "basic.h"
#include "matrix.h"
#include <stdio.h>
#include <string.h>
#ifndef DEBUG
#include "test2.h"
#else
int32_t matrix[4][4] = {{1, 3, 5, 9}, {1, 3, 1, 7}, {4, 3, 9, 7}, {5, 2, 0, 9}};
#endif

int main() {
    Matrix *m = create_mat(4, 4);
    memcpy(m->data, matrix, sizeof(matrix));
    i32 ans = det(m);
    printf("The matrix is :\n");
    print_mat(m);
    printf("The determinant is %d.\n", ans);
    free_mat(&m);
    return 0;
}