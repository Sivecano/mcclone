//
// Created by sivecano on 29/05/2021.
//

#include "UI.h"

void UI::render() {
    for (UIElement* e : elements)
        e->render();
}
