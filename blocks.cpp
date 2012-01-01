#include <GL/glew.h>
#include "blocks.h"

namespace Blocks {
    CubeBlock dirt("dirt", 0, 0, 0, 0, 0, 0);
    CubeBlock grass("grass", 1, 1, 1, 1, 2, 0);
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

void CubeBlock::render() {
    glDrawArrays(GL_QUADS, 0, 24);
}


