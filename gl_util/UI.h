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
    std::list<UIElement*> elements;
public:
    void addUIElement(UIElement* element);
    void render();
};

#endif //MCCLONE_UI_H
