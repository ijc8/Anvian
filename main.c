#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "util.h"

GLuint program;
GLuint attrCoords;
GLuint texture;
GLuint vbo;

GLfloat vertices[] = {
    0.5,  0.5,
    0.5, -0.5,
   -0.5, -0.5,
   -0.5,  0.5,
};

int initGL() {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, "standard.vert");
    GLuint fragShader = loadShader(GL_FRAGMENT_SHADER, "standard.frag");
    program = createProgram(vertexShader, fragShader);

    const char* attrName = "coord2d";
    attrCoords = glGetAttribLocation(program, attrName);
    if (attrCoords == -1) {
        fprintf(stderr, "Could not bind attribute %s\n", attrName);
        return 0;
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    texture = loadTexture("grass.png");

    return 1;
}

void display() {
    glClearColor(0.53, 0.8, 0.98, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glEnableVertexAttribArray(attrCoords);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(attrCoords, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_QUADS, 0, 4);
    glDisableVertexAttribArray(attrCoords);
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
