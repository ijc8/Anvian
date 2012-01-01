#ifndef _BLOCK_H
#define _BLOCK_H

class Block {
public:
    const char *name;

    Block(const char *name);
    virtual void render() = 0;
};

class CubeBlock : public Block {
public:
    int sides[6];

    CubeBlock(const char *name, int front, int left, int right,
              int back, int top, int bottom);
    virtual void render();
};

namespace Blocks {
    extern CubeBlock dirt;
    extern CubeBlock grass;
};

#endif
