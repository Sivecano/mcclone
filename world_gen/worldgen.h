//
// Created by sivecano on 04/05/2021.
//

#ifndef MCCLONE_WORLDGEN_H
#define MCCLONE_WORLDGEN_H
#include "ChunkSystem.h"
#include "glm/glm.hpp"

Chunk* generateChunk(glm::vec3 chunkpos)
{
    Chunk* out = new Chunk;
    out->chunkpos = chunkpos;

}


#endif //MCCLONE_WORLDGEN_H
