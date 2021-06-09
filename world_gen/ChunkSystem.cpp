//
// Created by sivecano on 01/05/2021.
//
#include "GL/glew.h"
#include "ChunkSystem.h"
#include "worldgen.cuh"
#include "bufferinterface.cuh"
#include "SDL2/SDL.h"
#include "glm/glm.hpp"

inline unsigned int bindex(unsigned int x, unsigned int y, unsigned int z)
{
    if (x > 16 || x < 0) SDL_Log("x out of bounds: %i", x);
    if (y > 16 || y < 0) SDL_Log("y out of bounds: %i", y);
    if (z > 16 || z < 0) SDL_Log("z out of bounds: %i", z);
    return x + 16 * z + 256 * y;
}

inline unsigned int bindex(glm::u8vec3 pos)
{
    return bindex(pos.x, pos.y, pos.z);
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

    SDL_Log("generating");
    chunks.push_back(generateChunk(chunkpos));

    return &(*chunks.back());
}


ChunkSystem::~ChunkSystem() {
    for (Chunk* chunk : chunks)
        killChunk(chunk);

    chunks.clear();
}
///TODO: get a better raytracing algorithm going
uint8_t ChunkSystem::getBlock(glm::ivec3 blockpos) {
    SDL_Log("getting block");
    glm::ivec3 chunkpos = glm::floor(glm::vec3(blockpos) / 16.f);
    return getChunk(chunkpos)->blockids[bindex(blockpos - chunkpos * 16)];
}

void ChunkSystem::setBlock(glm::ivec3 blockpos, uint8_t type) {
    glm::ivec3 chunkpos = glm::floor(glm::vec3(blockpos) / 16.f);
    getChunk(chunkpos)->blockids[bindex(blockpos - chunkpos * 16)] = type;
}

void update_Buffer(Chunk *chunk) {
    glNamedBufferSubData(chunk->buffer, 0, 4096, chunk->blockids);
    cube_facemask(chunk->buffer);
    cube_facemask(chunk->buffer);
}
