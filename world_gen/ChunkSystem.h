//
// Created by sivecano on 01/05/2021.
//

#ifndef MCCLONE_CHUNKSYSTEM_H
#define MCCLONE_CHUNKSYSTEM_H
#include "glm/vec3.hpp"

struct Chunk{
    Chunk* right;
    Chunk* left;
    Chunk* front;
    Chunk* back;
    Chunk* top;
    Chunk* bottom;
    glm::vec3 chunkpos;
    unsigned char blockids[4096];
};


class ChunkSystem {
    Chunk* current;
    

};

#endif //MCCLONE_CHUNKSYSTEM_H
