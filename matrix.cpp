#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "matrix.h"

#define I(mat, row, column) mat.data[row * mat.columns + column]

matrix makeMatrix(float *data, int rows, int columns) {
    matrix mat;
    mat.data = data;
    mat.rows = rows;
    mat.columns = columns;
    return mat;
}

matrix emptyMatrix(int rows, int columns) {
    float *data = (float *)malloc(rows * columns * sizeof(float));
    memset(data, 0, rows * columns * sizeof(float));
    return makeMatrix(data, rows, columns);
}

matrix identity(int size) {
    matrix result = emptyMatrix(size, size);
    int i;
    for (i = 0; i < size; i++)
        I(result, i, i) = 1;
    return result;
}

matrix multiply(matrix a, matrix b) {
    if (a.columns != b.rows)
        return (matrix){0};
    matrix result = makeMatrix((float *)malloc(a.rows * b.columns * sizeof(float)),
                               a.rows, b.columns);
    int r, c;
    for (r = 0; r < a.rows; r++) {
        for (c = 0; c < b.columns; c++) {
            float sum;
            int i;
            for (sum = i = 0; i < a.columns; i++)
                sum += I(a, r, i) * I(b, i, c);
            I(result, r, c) = sum;            
        }
    }
    return result;
}

matrix translate(float x, float y, float z) {
    matrix result = identity(4);

    I(result, 0, 3) = x;
    I(result, 1, 3) = y;
    I(result, 2, 3) = z;
    return result;
}

matrix perspective(float frustumScale, float zNear, float zFar) {
    matrix result = emptyMatrix(4, 4);

    I(result, 0, 0) = frustumScale;
    I(result, 1, 1) = frustumScale;
    I(result, 2, 2) = (zFar + zNear) / (zNear - zFar);
    I(result, 2, 3) = (2 * zFar * zNear) / (zNear - zFar);
    I(result, 3, 2) = -1;

    return result;
}

void printMatrix(matrix m) {
    int r, c;
    printf("[ ");
    for (c = 0; c < m.columns; c++)
        printf("%g ", I(m, 0, c));
    for (r = 1; r < m.rows; r++) {
        putchar('\n');
        printf("  ");
        for (c = 0; c < m.columns; c++)
            printf("%g ", I(m, r, c));
    }
    printf("]\n");
}
