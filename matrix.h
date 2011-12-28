typedef struct {
    float *data;
    int rows;
    int columns;
} matrix;

matrix makeMatrix(float *data, int rows, int columns);
matrix identity(int size);
matrix multiply(matrix a, matrix b);
matrix translate(float x, float y, float z);
matrix perspective(float frustumScale, float zNear, float zFar);
void printMatrix(matrix m);
