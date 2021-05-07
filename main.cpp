#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "shaders.h"
#include "rendering.h"
#include "Camera.h"
#include "ChunkSystem.h"
#include "glm/glm.hpp"

#define WIDTH 1280
#define HEIGHT 720

Camera playcam;
const uint8_t* keystate = SDL_GetKeyboardState(NULL);

int main()
{
    SDL_Window* win;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("we fucked up at init: %s", SDL_GetError());
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        SDL_Log("couldn't initialize SDL_image: %s", IMG_GetError());
        return 1;
    }


    win = SDL_CreateWindow("opengl with SDL2 :)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

    if (win == NULL)
    {
        SDL_Log("window creation failed: %s", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    ChunkSystem world;


    render_init(win);
    Chunk* basechunk = world.getChunk(glm::ivec3(0,0,0));
    basechunk->chunkpos = glm::vec3(0,0,0);
    playcam.pitch = 0;
    playcam.yaw = 0;
    playcam.direction = glm::vec3(0,0, -1);
    playcam.position = glm::vec3(0,0,-1);
    playcam.FOV = 0.1;
    playcam.zoom = 1;

    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
            for (int k = 0; k < 16; k++)
                basechunk->blockids[i + 16 * j + 256 * k] = (k==0) ? 7 : 0;

    basechunk->blockids[4 + 4 * 16 + 256] = 6;
    basechunk->blockids[4 + 4 * 16 + 2 * 256] = 6;
    basechunk->blockids[4 + 4 * 16 + 3 * 256] = 6;
    basechunk->blockids[4 + 4 * 16 + 4 * 256] = 6;
    basechunk->blockids[4 + 4 * 16 + 5 * 256] = 6;
    basechunk->blockids[3 + 4 * 16 + 1 * 256] = 6;
    basechunk->blockids[5 + 4 * 16 + 1 * 256] = 6;

    bool running = true;
    //TODO: clean up input system
    while(running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYUP:
                    if (e.key.keysym.sym == SDLK_ESCAPE)
                        SDL_SetRelativeMouseMode((SDL_GetRelativeMouseMode() == SDL_TRUE) ? SDL_FALSE : SDL_TRUE);
                    break;

                case SDL_MOUSEMOTION:
                    if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
                        playcam.yaw -= 2 * e.motion.xrel / 1280.;
                        playcam.pitch -= 2 * e.motion.yrel / 720.;
                        if (playcam.pitch > 1.57)
                            playcam.pitch = 1.57;
                        if (playcam.pitch < -1.57)
                            playcam.pitch = -1.57;
                        playcam.direction = glm::normalize(
                                glm::vec3(cos(playcam.pitch) * cos(playcam.yaw), sin(playcam.pitch),
                                          sin(playcam.yaw) * cos(playcam.pitch)));

                    }
                    break;

                case SDL_MOUSEWHEEL:
                    playcam.zoom += e.wheel.y / 100;


            };


        }

        playcam.position += glm::normalize(
                glm::cross(glm::vec3(0, 1, 0), playcam.direction - glm::vec3(0, playcam.direction.y, 0))) *
                            (0.1f * (keystate[SDL_SCANCODE_D] - keystate[SDL_SCANCODE_A]));
        playcam.position +=
                glm::vec3(0, 1, 0) * (0.1f * (keystate[SDL_SCANCODE_SPACE] - keystate[SDL_SCANCODE_LSHIFT]));
        playcam.position += glm::normalize(playcam.direction - glm::vec3(0, playcam.direction.y, 0)) *
                            (0.1f * (keystate[SDL_SCANCODE_W] - keystate[SDL_SCANCODE_S]));


        playcam.FOV += 0.01 * (keystate[SDL_SCANCODE_E] - keystate[SDL_SCANCODE_Q]);
        playcam.FOV = fmin(3.14159, playcam.FOV);
        playcam.FOV = fmax(0.1, playcam.FOV);

        SDL_Log("x: %f, y: %f, z: %f, fov: %f", playcam.position.x, playcam.position.y, playcam.position.z,
                playcam.FOV);

        //render(win);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int i = -2; i < 3; i++)
            for (int j = -2; j < 3; j++)
                for (int k = 0; k < 2; k++)
                    renderChunk(playcam, world.getChunk(glm::ivec3(i, k, j)));

        SDL_GL_SwapWindow(win);

    }
    render_quit();
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
    
}