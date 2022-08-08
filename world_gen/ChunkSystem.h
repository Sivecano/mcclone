//
// Created by sivecano on 01/05/2021.
//

#ifndef MCCLONE_CHUNKSYSTEM_H
#define MCCLONE_CHUNKSYSTEM_H
#include "glm/vec3.hpp"
#include <vector>
#include <utility>
#include <cstdint>

//TODO: implement some chunk system;

struct Chunk{
    glm::ivec3 chunkpos;
    unsigned char blockids[4096];
    unsigned int buffer;
};

void update_Buffer(Chunk* chunk);

class ChunkSystem {
public:
    std::vector<Chunk*> chunks;
    Chunk* getChunk(glm::ivec3 chunkpos);
    uint8_t getBlock(glm::ivec3 blockpos);
    void setBlock(glm::ivec3 blockpos, uint8_t type);
    ~ChunkSystem();

};

#endif //MCCLONE_CHUNKSYSTEM_H
