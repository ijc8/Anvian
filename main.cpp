#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "chunk.h"

#define TEXMAP_WIDTH 2
#define TEXMAP_HEIGHT 2

GLuint program;
GLuint coordAttr, texcoordAttr;
GLuint texture;
GLuint textureUniform;
GLuint cameraMatUniform;
GLuint worldMatUniform;
GLuint vbo, vboTexcoords;

glm::mat4 projection;

glm::vec3 cameraPos(-2.5f, 0.0f, 0.0f);
glm::vec3 cameraSpherePos(0.0f, 0.0f, 0.5f);

Chunk chunk(0, 0, 0);

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

/* order: front, left, right, back, top, and bottom */
int sides[] = { 1, 1, 1, 1, 2, 0 };

GLfloat texcoords[4 * 2 * 6];

glm::mat4 calcLookAtMatrix(const glm::vec3 &cameraPt, const glm::vec3 &centerPt, const glm::vec3 &upPt) {
    glm::vec3 lookDir = glm::normalize(cameraPt - centerPt);
    glm::vec3 upDir = glm::normalize(upPt);

    glm::vec3 rightDir = glm::normalize(glm::cross(lookDir, upDir));
    glm::vec3 perpUpDir = glm::cross(rightDir, lookDir);

    glm::mat4 rotMat(1.0f);
    rotMat[0] = glm::vec4(rightDir, 0.0f);
    rotMat[1] = glm::vec4(perpUpDir, 0.0f);
    rotMat[2] = glm::vec4(-lookDir, 0.0f);

    rotMat = glm::transpose(rotMat);

    glm::mat4 transMat(1.0f);
    transMat[3] = glm::vec4(-cameraPt, 1.0f);

    return rotMat * transMat;
}

glm::vec3 calcCameraPosition() {
    return sphericalToEuclidean(cameraSpherePos) + cameraPos;
}

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

    worldMatUniform = glGetUniformLocation(program, "worldMatrix");
    cameraMatUniform = glGetUniformLocation(program, "cameraMatrix");

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return 1;
}

void reshape(int w, int h) {
    projection = glm::perspective(90.0f, (float)w / h, 0.1f, 100.0f);
    glViewport(0, 0, w, h);
    glutPostRedisplay();
}

void display() {
    glm::mat4 cameraMatrix = projection * calcLookAtMatrix(calcCameraPosition(), cameraPos, glm::vec3(0, 1, 0));

    glClearColor(0.53, 0.8, 0.98, 1.0);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    glUniformMatrix4fv(cameraMatUniform, 1, GL_FALSE, glm::value_ptr(cameraMatrix));

    glEnableVertexAttribArray(coordAttr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(coordAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glUniform1i(textureUniform, 0);
    glEnableVertexAttribArray(texcoordAttr);
    glBindBuffer(GL_ARRAY_BUFFER, vboTexcoords);
    glVertexAttribPointer(texcoordAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);

    chunk.render();

    glDisableVertexAttribArray(coordAttr);
    glDisableVertexAttribArray(texcoordAttr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        cameraPos += (calcCameraPosition() - cameraPos) * 0.1f;
        break;
    case 's':
        cameraPos -= (calcCameraPosition() - cameraPos) * 0.1f;
        break;
    case 'a':
        cameraPos -= sphericalToEuclidean(glm::vec3(cameraSpherePos.x + 90, 0, cameraSpherePos.z)) * 0.1f;
        break;
    case 'd':
        cameraPos += sphericalToEuclidean(glm::vec3(cameraSpherePos.x + 90, 0, cameraSpherePos.z)) * 0.1f;
        break;
    };

    glutPostRedisplay();
}

void keyboardSpecial(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        cameraSpherePos.y -= 2;
        break;
    case GLUT_KEY_DOWN:
        cameraSpherePos.y += 2;
        break;
    case GLUT_KEY_LEFT:
        cameraSpherePos.x -= 2;
        break;
    case GLUT_KEY_RIGHT:
        cameraSpherePos.x += 2;
    };

    glutPostRedisplay();
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
    if (!GLEW_VERSION_2_0)
        fprintf(stderr, "You don't even support OpenGL 2.0?! Well, this almost certainly won't work, then.\n");

    if (!initGL())
        return 1;

    chunk.blocks[0][0][0] = 1;
    chunk.blocks[0][0][1] = 1;
    chunk.blocks[0][1][0] = 1;
    chunk.blocks[1][0][0] = 1;
    chunk.blocks[3][3][3] = 1;

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutMainLoop();

    return 0;
}
