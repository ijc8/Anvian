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

matrix identity(int size) {
    float *data = malloc(size * size * sizeof(float));
    memset(data, 0, size * size * sizeof(float));
    int i;
    for (i = 0; i < size; i++)
        data[i*size+i] = 1;
    return makeMatrix(data, size, size);
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
