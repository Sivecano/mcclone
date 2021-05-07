//
// Created by sivecano on 01/05/2021.
//

#ifndef MCCLONE_CHUNKSYSTEM_H
#define MCCLONE_CHUNKSYSTEM_H
#include "glm/vec3.hpp"
#include <vector>
#include <utility>

//TODO: implement some chunk system;

struct Chunk{
    glm::ivec3 chunkpos;
    unsigned char blockids[4096];
    unsigned int buffer;
};


class ChunkSystem {
public:
    std::vector<Chunk*> chunks;
    Chunk* getChunk(glm::ivec3 chunkpos);
    ~ChunkSystem();

};

#endif //MCCLONE_CHUNKSYSTEM_H
