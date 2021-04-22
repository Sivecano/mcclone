#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#define WIDTH 800
#define HEIGHT 400



int main(int argc, char* argv[])
{
    SDL_Window* win;
    SDL_GLContext con;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("we fucked up at init: %s", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);


    win = SDL_CreateWindow("opengl with SDL2 :)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

    if (win == NULL)
    {
        SDL_Log("window creation failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    con = SDL_GL_CreateContext(win);
    if(con == NULL)
    {
        SDL_Log("error creating opengl context: %s", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_GL_MakeCurrent(win, con);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        SDL_Log("glew initilisation failed");
        SDL_GL_DeleteContext(con);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    
    bool running = true;

    while(running)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false;
        }
        glClearColor(.2f, .2f, .8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(win);
    }


    SDL_GL_DeleteContext(con);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
    
}