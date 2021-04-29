//
// Created by sivecano on 29/04/2021.
//
#include "rendering.h"
#include "gl_util/shaders.h"

SDL_GLContext con;
int width;
int height;
GLuint shaderprogramm;

void render_init(SDL_Window* win)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    con = SDL_GL_CreateContext(win);
    if(con == NULL)
    {
        SDL_Log("error creating opengl context: %s", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        exit(1);
    }

    SDL_GL_MakeCurrent(win, con);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        SDL_Log("glew initilisation failed");
        SDL_GL_DeleteContext(con);
        SDL_DestroyWindow(win);
        SDL_Quit();
        exit(1);
    }

    SDL_GetWindowSize(win, &width, &height);

    glViewport(0, 0, width, height);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(.6f, .6f, 1.0f, 1.0f);

    shaderprogramm = shader_program("shaders/vertexshader.vert.glsl",
                                    "shaders/fragmentshader.frag.glsl",
                                    "shaders/geometryshader.geo.glsl");

}

void render_quit()
{
    SDL_GL_DeleteContext(con);

}


void render(SDL_Window* win)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderprogramm);


    SDL_GL_SwapWindow(win);
}


