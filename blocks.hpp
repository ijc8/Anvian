#ifndef _BLOCK_H
#define _BLOCK_H

#include <GL/glew.h>

#define TEXMAP_WIDTH 2
#define TEXMAP_HEIGHT 2

class Block {
public:
    const char *name;

    Block(const char *name);
    virtual void init() = 0;
    virtual void render() = 0;
};

class CubeBlock : public Block {
public:
    int sides[6];
    GLuint vboTexcoords;

    CubeBlock(const char *name, int front, int left, int right,
              int back, int top, int bottom);
    virtual void init();
    void buildTexcoords();
    virtual void render();
};

namespace Blocks {
    extern CubeBlock dirt;
    extern CubeBlock grass;
    extern CubeBlock stone;

    void init();
};

#endif
