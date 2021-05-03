//
// Created by sivecano on 29/04/2021.
//
#include "rendering.h"
#include "gl_util/shaders.h"
#include "SDL2/SDL_image.h"
#include "ChunkSystem.h"
#include "Camera.h"
#include "cudaKernels/kernels.cuh"

SDL_GLContext con;
int width;
int height;
GLuint block_shader;

GLuint block_buffer; // the first 4096 elements are the block types within the chunks while the other 4096 are the block face masks
GLuint vertex_array;
GLuint block_texture_atlas;




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
    glEnable(GL_DEPTH_TEST);
    glClearColor(.6f, .6f, 1.0f, 1.0f);

    block_shader = shader_program("shaders/vertexshader.vert.glsl",
                                  "shaders/fragmentshader.frag.glsl",
                                  "shaders/geometryshader.geo.glsl");

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    glGenTextures(1, &block_texture_atlas);
    SDL_Surface* tex = IMG_Load("./textures/tests.png");
    if (tex == NULL)
    {
        SDL_Log("texture didn't load: %s", IMG_GetError());
        exit(1);
    }

    glBindTexture(GL_TEXTURE_2D, block_texture_atlas);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);

    SDL_FreeSurface(tex);


    glGenBuffers(1, &block_buffer);

    unsigned char points[4096] = {
            6, 2, 3, 5, 4, 1
    };
    memset(points + 6, 0, 4090);

    glBindBuffer(GL_ARRAY_BUFFER, block_buffer);
    glBufferData(GL_ARRAY_BUFFER, 8192, NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 6, points);

    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLint posAttrib = glGetAttribLocation(block_shader, "ftype");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribIPointer(posAttrib, 1, GL_UNSIGNED_BYTE, 0, NULL);

    posAttrib = glGetAttribLocation(block_shader, "ffacemask");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribIPointer(posAttrib, 1, GL_UNSIGNED_BYTE, 0, (void*)4096);

    cudainit();
    register_blockbuffer(block_buffer);
}

void render_quit() {
    unregister_blockbuffer();
    SDL_GL_DeleteContext(con);
}

void render(SDL_Window* win)
{
    static uint32_t last;

    //SDL_Log("%i", SDL_GetTicks() - last);
    last = SDL_GetTicks();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniform3f(glGetUniformLocation(block_shader, "chunkpos"), -1, .5, 0.7);
    glUniform1i(glGetUniformLocation(block_shader, "time"), SDL_GetTicks());

    cube_facemask();
    glUseProgram(block_shader);
    glBindBuffer(GL_ARRAY_BUFFER, block_buffer);
    glBindVertexArray(vertex_array);

    glDrawArrays(GL_POINTS, 0, 4096);


    SDL_GL_SwapWindow(win);
}

void renderChunk(Camera cam, Chunk chunk)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(block_shader);
    glUniform3f(glGetUniformLocation(block_shader, "chunkpos"), chunk.chunkpos.x, chunk.chunkpos.y, chunk.chunkpos.z);
    glUniform3f(glGetUniformLocation(block_shader, "campos"), cam.position.x, cam.position.y, cam.position.z);
    glUniform3f(glGetUniformLocation(block_shader, "camdir"), cam.direction.x, cam.direction.y, cam.direction.z);
    glUniform1f(glGetUniformLocation(block_shader, "fov"), cam.FOV);
    glUniform1f(glGetUniformLocation(block_shader, "zoom"), cam.zoom);
    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, block_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 4096, chunk.blockids);
    cube_facemask();
    glDrawArrays(GL_POINTS, 0, 4096);
    //glVertexAttribPointer()


}


