//
// Created by sivecano on 08/05/2021.
//

#ifndef MCCLONE_PLAYER_H
#define MCCLONE_PLAYER_H
#include "glm/glm.hpp"
#include "ChunkSystem.h"


class Player {

};

float raycast_distance(glm::vec3 from, glm::vec3 direction, ChunkSystem* world, float maxrange);

glm::ivec3 raycast_block(glm::vec3 direction, glm::vec3 start, ChunkSystem* world, float maxrange);



#endif //MCCLONE_PLAYER_H
