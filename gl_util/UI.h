//
// Created by sivecano on 29/05/2021.
//

#ifndef MCCLONE_UI_H
#define MCCLONE_UI_H
#include <list>

class UIElement
{
public:
    virtual void render() = 0;
};


class UI {
    unsigned int UIshader;
    std::list<UIElement*> elements;
public:
    UI();
    void addUIElement(UIElement* element);
    void render();
};

class ImageElement : public UIElement
{
private:
    unsigned int textureBuffer;
    unsigned int vertexBuffer;
public:
    ImageElement(const char* image, float x, float y, float h, float w);
    void render() override;
};

#endif //MCCLONE_UI_H
