//
// Created by sivecano on 01/05/2021.
//

#ifndef MCCLONE_CAMERA_H
#define MCCLONE_CAMERA_H

#include "glm/vec3.hpp"


struct Camera {
    glm::vec3 position;
    glm::vec3 direction;
    float FOV;
    float zoom;
};


#endif //MCCLONE_CAMERA_H
