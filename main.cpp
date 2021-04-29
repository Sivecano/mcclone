#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "shaders.h"
#include "rendering.h"

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


    win = SDL_CreateWindow("opengl with SDL2 :)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

    if (win == NULL)
    {
        SDL_Log("window creation failed: %s", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    render_init(win);


    bool running = true;

    while(running)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false;
        }

        render(win);

    }

    render_quit();
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
    
}