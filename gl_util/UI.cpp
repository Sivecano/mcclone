//
// Created by sivecano on 29/05/2021.
//

#include "UI.h"
#include "GL/glew.h"
#include "SDL2/SDL_image.h"
#include "shaders.h"


UI::UI() {
    //UIshader = shader_program("shaders/UIshader.vert.glsl", "shaders/UIshader.frag.glsl");
}

void UI::render() {
    glUseProgram(UIshader);
    for (UIElement* e : elements)
        e->render();
}

void UI::addUIElement(UIElement *element) {
    elements.push_back(element);
}

ImageElement::ImageElement(const char *image, float x, float y, float h, float w) {


    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    float pos[12]{
        x, y, 0,
        x + w, y, 0,
        x + w, y + h, 0,
        x, y + h, 0
        };
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

    glGenTextures(1, &textureBuffer);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);

    SDL_Surface* tex = IMG_Load(image);
    if (tex == NULL)
    {
        SDL_Log("texture didn't load: %s", IMG_GetError());
        exit(1);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);

    SDL_FreeSurface(tex);

}

void ImageElement::render() {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);
    glDrawArrays(GL_QUADS, 0, 1);
}
