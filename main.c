#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <assert.h>
#include "util.h"

#define TEXMAP_WIDTH 2
#define TEXMAP_HEIGHT 1

GLuint program;
GLuint coordAttr, texcoordAttr;
GLuint grassSideTex, grassTopTex;
GLuint textureUniform;
GLuint vbo, vboTexcoords;

GLuint zNearUniform;
GLuint zFarUniform;
GLuint frustumScaleUniform;

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
};

/* order: front, left, right, back, top (and bottom, when that's added) */
int sides[] = { 0, 0, 0, 0, 1 };

GLfloat texcoords[4 * 2 * 5];

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

    textureUniform = glGetUniformLocation(program, "mytexture");
    zNearUniform = glGetUniformLocation(program, "zNear");
    zFarUniform = glGetUniformLocation(program, "zFar");
    frustumScaleUniform = glGetUniformLocation(program, "frustumScale");

    assert(coordAttr != -1);
    assert(texcoordAttr != -1);
    assert(textureUniform != -1);
    assert(zNearUniform != -1);
    assert(zFarUniform != -1);
    assert(frustumScaleUniform != -1);

    glUseProgram(program);
    glUniform1f(frustumScaleUniform, 1.0f);
    glUniform1f(zNearUniform, 1.0f);
    glUniform1f(zFarUniform, 3.0f);
    glUseProgram(0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    int i;
    for (i = 0; i < 5; i++) {
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

    grassSideTex = loadTexture("terrain.png");

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

    glDrawArrays(GL_QUADS, 0, 20);
    glDisableVertexAttribArray(coordAttr);
    glDisableVertexAttribArray(texcoordAttr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Lyx Episode V: OpenGL Strikes Back!");

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    if (!initGL())
        return 1;

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
