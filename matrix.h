typedef struct {
    float *data;
    int rows;
    int columns;
} matrix;

matrix makeMatrix(float *data, int rows, int columns);
matrix identity(int size);
matrix multiply(matrix a, matrix b);
void printMatrix(matrix m);
