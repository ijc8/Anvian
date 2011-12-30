#include <glm/glm.hpp>

class Chunk {
    public:
        static const int XBlocks = 16;
        static const int YBlocks = 16;
        static const int ZBlocks = 16;

        int ***blocks;

        glm::ivec3 pos;

        Chunk(int x, int y, int z);
        void render();
};
