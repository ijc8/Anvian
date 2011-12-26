#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "util.h"

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
  -0.5, 0.5, 0.5,
};

GLfloat texcoords[] = {
    /* for each y, 1 - y was done to account for the bottom-left thing */
    0.0, 0.0,
    0.5, 0.0,
    0.5, 1.0,
    0.0, 1.0,

    /* repeat */
    0.0, 0.0,
    0.5, 0.0,
    0.5, 1.0,
    0.0, 1.0,

    0.0, 0.0,
    0.5, 0.0,
    0.5, 1.0,
    0.0, 1.0,

    0.0, 0.0,
    0.5, 0.0,
    0.5, 1.0,
    0.0, 1.0,

    0.5, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.5, 1.0,
};

int initGL() {
    glDisable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0, 1.0);

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, "standard.vert");
    GLuint fragShader = loadShader(GL_FRAGMENT_SHADER, "standard.frag");
    program = createProgram(vertexShader, fragShader);

    coordAttr = glGetAttribLocation(program, "coord");
    if (coordAttr == -1) {
        fprintf(stderr, "Could not bind attribute \"coord\"\n");
        return 0;
    }

    texcoordAttr = glGetAttribLocation(program, "texcoord");
    if (texcoordAttr == -1) {
        fprintf(stderr, "Could not bind attribute \"texcoord\"\n");
        return 0;
    }

    textureUniform = glGetUniformLocation(program, "mytexture");
    if (textureUniform == -1) {
        fprintf(stderr, "Could not bind uniform \"mytexture\"\n");
        return 0;
    }

    zNearUniform = glGetUniformLocation(program, "zNear");
    if (zNearUniform == -1) {
        fprintf(stderr, "Could not bind uniform \"zNear\"\n");
        return 0;
    }

    zFarUniform = glGetUniformLocation(program, "zFar");
    if (zFarUniform == -1) {
        fprintf(stderr, "Could not bind uniform \"zFar\"\n");
        return 0;
    }

    frustumScaleUniform = glGetUniformLocation(program, "frustumScale");
    if (frustumScaleUniform == -1) {
        fprintf(stderr, "Could not bind uniform \"frustumScale\"\n");
        return 0;
    }

    glUseProgram(program);
    glUniform1f(frustumScaleUniform, 1.0f);
    glUniform1f(zNearUniform, 1.0f);
    glUniform1f(zFarUniform, 3.0f);
    glUseProgram(0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
