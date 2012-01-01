#include "blocks.h"

namespace Blocks {
    Block dirt("dirt", 0, 0, 0, 0, 0, 0);
    Block grass("grass", 1, 1, 1, 1, 2, 0);
};

Block::Block(char *name, int front, int left, int right,
             int back, int top, int bottom) {
    this->name = name;
    sides[0] = front;
    sides[1] = left;
    sides[2] = right;
    sides[3] = back;
    sides[4] = top;
    sides[5] = bottom;
}
