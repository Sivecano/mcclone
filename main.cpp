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


    render_init(win);
    Chunk basechunk;
    basechunk.chunkpos = glm::vec3(0,0,0);
    playcam.direction = glm::vec3(0, 0, 1);
    playcam.FOV = 90;


    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
            basechunk.blockids[i + 256 * j] = 1;

    bool running = true;

    while(running)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false;
        }

        playcam.position += glm::vec3(0.1 * (keystate[SDL_SCANCODE_D] - keystate[SDL_SCANCODE_A]),
                                      0.1 * (keystate[SDL_SCANCODE_SPACE] - keystate[SDL_SCANCODE_LSHIFT]),
                                      0.1 * (keystate[SDL_SCANCODE_W] - keystate[SDL_SCANCODE_S]));

        //render(win);
        renderChunk(playcam, basechunk);

        SDL_GL_SwapWindow(win);

    }

    render_quit();
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
    
}