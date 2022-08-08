//
// Created by sivecano on 08/05/2021.
//

#include "Player.h"
#include "glm/vec3.hpp"
#include "ChunkSystem.h"
#include "SDL2/SDL.h"

// false means -1 true means 1
inline float dtoi(float in, bool dir)
{
    if (dir)
        return ceilf(in) - in;
    else
        return floorf(in) - in;
}

float raycast_distance(glm::vec3 from, glm::vec3 direction, ChunkSystem* world, float maxrange)
{
    SDL_Log("here");
    direction = glm::normalize(direction);

    //float dd = 1.f / direction.x;
    glm::vec3 curr = from;
    float lasty = roundf(curr.y);
    float lastz = roundf(curr.z);
    float dist = 0;

    //ssscurr += direction * (dtoi(curr.x, direction.x > 0) / direction.x);
    SDL_Log("there");
    while (dist < maxrange && world->getBlock(glm::ivec3(glm::round(curr))) == 0)
    {
        curr += direction;

        dist += 1;
    }
    SDL_Log("dist: %f", dist);

    return dist;
}

glm::ivec3 raycast_block(glm::vec3 from, glm::vec3 direction, ChunkSystem* world, float maxrange)
{
    return glm::ivec3(glm::round(from + glm::normalize(direction) * raycast_distance(from, direction, world, maxrange)));
}