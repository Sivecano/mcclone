//
// Created by sivecano on 01/05/2021.
//
#include "GL/glew.h"
#include "ChunkSystem.h"
#include "worldgen.cuh"
#include "bufferinterface.cuh"

Chunk* generateChunk(glm::ivec3 chunkpos)
{
    Chunk* out = new Chunk;
    out->chunkpos = chunkpos;
    glGenBuffers(1, &out->buffer);
    glBindBuffer(GL_ARRAY_BUFFER, out->buffer);
    glBufferData(GL_ARRAY_BUFFER, 8192, NULL, GL_DYNAMIC_DRAW);
    register_buffer(out->buffer);
    generate_chunkids(out->blockids, out->chunkpos);

    return out;

}

void killChunk(Chunk* chunk)
{
    unregister_buffer(chunk->buffer);
    glDeleteBuffers(1, &chunk->buffer);
    delete chunk;
}

Chunk *ChunkSystem::getChunk(glm::ivec3 chunkpos) {

    for (Chunk* chunk : chunks)
        if (chunk->chunkpos == chunkpos)
            return chunk;

    chunks.push_back(generateChunk(chunkpos));

    return chunks.back();
}


ChunkSystem::~ChunkSystem() {
    for (Chunk* chunk : chunks)
        killChunk(chunk);

    chunks.clear();
}

