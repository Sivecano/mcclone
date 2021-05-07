//
// Created by sivecano on 29/04/2021.
//

#ifndef MCCLONE_RENDERING_H
#define MCCLONE_RENDERING_H
#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "shaders.h"
#include "Camera.h"
#include "ChunkSystem.h"

void render(SDL_Window* win);
void render_init(SDL_Window* win);
void render_quit();
void renderChunk(Camera cam, Chunk* chunk);

#endif //MCCLONE_RENDERING_H
