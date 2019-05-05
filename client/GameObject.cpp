#include <emscripten.h>
#include <emscripten/bind.h>
#include "objects.h"

using namespace G;

GameObject::GameObject(const int startX, const int startY): 
    x(startX),
    y(startY)
{
    still = false;
};

void GameObject::moveDown() {
    y += 1;
};

int GameObject::getX() const {
    return x;
};

int GameObject::getY() const {
    return y;
};

void GameObject::setX(const int newX) {
    x = newX;
};

void GameObject::setY(const int newY) {
    y = newY;
};

bool GameObject::isStill() const {
    return still;
};

void GameObject::setStill() {
    still = true;
};