#ifndef _BLOCK_H
#define _BLOCK_H

class Block {
 public:
    char *name;
    int sides[6];

    Block(char *name, int front, int left, int right,
          int back, int top, int bottom);
};

namespace Blocks {
    extern Block dirt;
    extern Block grass;
};

#endif
