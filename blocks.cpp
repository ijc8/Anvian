#include <GL/glew.h>
#include "blocks.h"
#include "main.h"

#include <stdio.h>

namespace Blocks {
    CubeBlock dirt("dirt", 0, 0, 0, 0, 0, 0);
    CubeBlock grass("grass", 1, 1, 1, 1, 2, 0);
    Block *blocks[] = { &dirt, &grass };

    void init() {
        for (int i = 0; i < 2; i++)
            blocks[i]->init();
    }
        
};

Block::Block(const char *name) {
    this->name = name;
}

CubeBlock::CubeBlock(const char *name, int front, int left, int right,
                     int back, int top, int bottom) : Block(name) {
    sides[0] = front;
    sides[1] = left;
    sides[2] = right;
    sides[3] = back;
    sides[4] = top;
    sides[5] = bottom;
}

void CubeBlock::init() {
    buildTexcoords();
}

void CubeBlock::buildTexcoords() {
    GLfloat texcoords[4 * 2 * 6];

    for (int i = 0; i < 6; i++) {
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
}

void CubeBlock::render() {
    glBindBuffer(GL_ARRAY_BUFFER, vboTexcoords);
    glVertexAttribPointer(texcoordAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_QUADS, 0, 24);
}


