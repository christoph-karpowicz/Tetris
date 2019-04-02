#include <emscripten.h>
#include <emscripten/bind.h>
#include "objects.h"

using namespace G;

GameObject::GameObject(int startX, int startY): 
    x(startX),
    y(startY)
{
    still = false;
};

void GameObject::moveDown() {
    y += 1;
};

bool GameObject::isStill() {
    return still;
};

void GameObject::setStill() {
    still = true;
};

int* GameObject::bottomRightCoordinates() {
    return 0;
};