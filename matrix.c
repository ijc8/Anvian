#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "matrix.h"

matrix makeMatrix(float *data, int rows, int columns) {
    matrix mat;
    mat.data = data;
    mat.rows = rows;
    mat.columns = columns;
    return mat;
}

matrix emptyMatrix(int rows, int columns) {
    float *data = malloc(rows * columns * sizeof(float));
    memset(data, 0, rows * columns * sizeof(float));
    return makeMatrix(data, rows, columns);
}

matrix identity(int size) {
    matrix result = emptyMatrix(size, size);
    int i;
    for (i = 0; i < size; i++)
        result.data[i*size+i] = 1;
    return result;
}

matrix multiply(matrix a, matrix b) {
    if (a.columns != b.rows)
        return (matrix){0};
    float *data = malloc(a.rows * b.columns * sizeof(float));
    int r, c;
    for (r = 0; r < a.rows; r++) {
        for (c = 0; c < b.columns; c++) {
            float sum;
            int i;
            for (sum = i = 0; i < a.columns; i++)
                sum += a.data[r*a.columns+i] * b.data[i*b.columns+c];
            data[r*b.columns+c] = sum;
        }
    }
    return makeMatrix(data, a.rows, b.columns);          
}

matrix translate(float x, float y, float z) {
    matrix result = identity(4);
    result.data[4*0+3] = x;
    result.data[4*1+3] = y;
    result.data[4*2+3] = z;
    return result;
}

matrix perspective(float frustumScale, float zNear, float zFar) {
    matrix result = emptyMatrix(4, 4);

    result.data[4*0+0] = frustumScale;
    result.data[4*1+1] = frustumScale;
    result.data[4*2+2] = (zFar + zNear) / (zNear - zFar);
    result.data[4*2+3] = (2 * zFar * zNear) / (zNear - zFar);
    result.data[4*3+2] = -1;

    return result;
}

void printMatrix(matrix m) {
    int r, c;
    printf("[ ");
    for (c = 0; c < m.columns; c++)
            printf("%g ", m.data[c]);
    for (r = 1; r < m.rows; r++) {
        putchar('\n');
        printf("  ");
        for (c = 0; c < m.columns; c++) {
            printf("%g ", m.data[r*m.columns+c]);
        }
    }
    printf("]\n");
}
