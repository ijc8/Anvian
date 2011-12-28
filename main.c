#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "util.h"
#include "matrix.h"

#define TEXMAP_WIDTH 2
#define TEXMAP_HEIGHT 2

GLuint program;
GLuint coordAttr, texcoordAttr;
GLuint texture;
GLuint textureUniform;
GLuint vbo, vboTexcoords;

GLuint matrixUniform;

GLfloat vertices[] = {
    /* front */
   -0.5,  0.5, 0.5,
    0.5,  0.5, 0.5,
    0.5, -0.5, 0.5,
   -0.5, -0.5, 0.5,

    /* left */
   -0.5,  0.5, -0.5,
   -0.5,  0.5,  0.5,
   -0.5, -0.5,  0.5,
   -0.5, -0.5, -0.5,
    
    /* right */
    0.5,  0.5,  0.5,
    0.5,  0.5, -0.5,
    0.5, -0.5, -0.5,
    0.5, -0.5,  0.5,

    /* back */
    0.5,  0.5, -0.5,
   -0.5,  0.5, -0.5,
   -0.5, -0.5, -0.5,
    0.5, -0.5, -0.5,

    /* top (uses different texture) */
   -0.5, 0.5, -0.5,
    0.5, 0.5, -0.5,
    0.5, 0.5,  0.5,
   -0.5, 0.5,  0.5,

    /* bottom (uses different texture) */
   -0.5, -0.5,  0.5,
    0.5, -0.5,  0.5,
    0.5, -0.5, -0.5,
   -0.5, -0.5, -0.5,
};

/* order: front, left, right, back, top (and bottom, when that's added) */
int sides[] = { 1, 1, 1, 1, 2, 0 };

GLfloat texcoords[4 * 2 * 6];

int initGL() {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0, 1.0);

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, "standard.vert");
    GLuint fragShader = loadShader(GL_FRAGMENT_SHADER, "standard.frag");
    program = createProgram(vertexShader, fragShader);

    coordAttr = glGetAttribLocation(program, "coord");
    texcoordAttr = glGetAttribLocation(program, "texcoord");

    matrixUniform = glGetUniformLocation(program, "matrix");

    assert(coordAttr != -1);
    assert(texcoordAttr != -1);
    assert(matrixUniform != -1);

    int frustumScale = 1;
    int zNear = 1;
    int zFar = 3;

    matrix m = perspective(frustumScale, zNear, zFar);

    glUseProgram(program);
    glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, m.data);
    glUseProgram(0);

    free(m.data);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    int i;
    for (i = 0; i < 6; i++) {
        float x = (float)(sides[i] % TEXMAP_WIDTH) / TEXMAP_WIDTH;
        float endX = x + (1.0 / TEXMAP_WIDTH);
        float y = (float)(sides[i] / TEXMAP_WIDTH) / TEXMAP_WIDTH;
        float endY = y + (1.0 / TEXMAP_HEIGHT);

        texcoords[i*8+0] = x;
        texcoords[i*8+1] = y;

        texcoords[i*8+2] = endX;
        texcoords[i*8+3] = y;

        texcoords[i*8+4] = endX;
        texcoords[i*8+5] = endY;

        texcoords[i*8+6] = x;
        texcoords[i*8+7] = endY;
    }

    glGenBuffers(1, &vboTexcoords);
    glBindBuffer(GL_ARRAY_BUFFER, vboTexcoords);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    texture = loadTexture("terrain.png");

    return 1;
}

void display() {
    glClearColor(0.53, 0.8, 0.98, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(coordAttr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(coordAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glUniform1i(textureUniform, 0);
    glEnableVertexAttribArray(texcoordAttr);
    glBindBuffer(GL_ARRAY_BUFFER, vboTexcoords);
    glVertexAttribPointer(texcoordAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_QUADS, 0, 24);
    glDisableVertexAttribArray(coordAttr);
    glDisableVertexAttribArray(texcoordAttr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Anvian");

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    if (!initGL())
        return 1;

    float data1[2][3] = {
        {  1, 0, 2 },
        { -1, 3, 1 },
    };

    float data2[3][2] = {
        { 3, 1 },
        { 2, 1 },
        { 1, 0 },        
    };

    matrix m1 = makeMatrix((float *)data1, 2, 3);
    matrix m2 = makeMatrix((float *)data2, 3, 2);
    puts("m1:");
    printMatrix(m1);
    puts("m2:");
    printMatrix(m2);
    putchar('\n');

    /* NOTE: Don't do what happens next. Could easily cause a memory leak,
     * because multiply() makes a new matrix with its own malloc()'d space.
     */
    puts("m1 * m2:");
    printMatrix(multiply(m1, m2));
    puts("m2 * m1:");
    printMatrix(multiply(m2, m1));
    putchar('\n');

    matrix m = identity(4);
    puts("identity(4):");
    printMatrix(m);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
