//
// Created by sivecano on 01/05/2021.
//
#include "GL/glew.h"
#include "ChunkSystem.h"
#include "worldgen.cuh"
#include "bufferinterface.cuh"

inline unsigned int bindex(unsigned int x, unsigned int y, unsigned int z)
{
    return x + 16 * z + 256 * y;
}

inline unsigned int bindex(glm::ivec3 pos)
{
    return pos.x + 16 * pos.z + 256 * pos.y;
}


Chunk* generateChunk(glm::ivec3 chunkpos)
{
    Chunk* out = new Chunk;
    out->chunkpos = chunkpos;
    glGenBuffers(1, &out->buffer);
    glBindBuffer(GL_ARRAY_BUFFER, out->buffer);
    glBufferData(GL_ARRAY_BUFFER, 8192, NULL, GL_DYNAMIC_DRAW);
    register_buffer(out->buffer);
    generate_chunkids(out->blockids, out->chunkpos);

    update_Buffer(out);

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

uint8_t ChunkSystem::getBlock(glm::ivec3 blockpos) {
    return getChunk(glm::ivec3(blockpos / 16))->blockids[bindex(blockpos % 16)];
}

void ChunkSystem::setBlock(glm::ivec3 blockpos, uint8_t type) {
    getChunk(glm::ivec3(blockpos / 16))->blockids[bindex(blockpos % 16)] = type;
}

void update_Buffer(Chunk *chunk) {
    glNamedBufferSubData(chunk->buffer, 0, 4096, chunk->blockids);
    cube_facemask(chunk->buffer);
}
