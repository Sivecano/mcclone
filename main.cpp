#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#define WIDTH 800
#define HEIGHT 400



int main(int argc, char* argv[])
{
    SDL_Window* win;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("we fucked up at init: %s", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 6);


    win = SDL_CreateWindow("opengl with SDL2 :)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

    if (win == nullptr)
    {
        SDL_Log("window creation failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }


    SDL_Delay(1000);


    SDL_Quit();
    return 0;
    
}