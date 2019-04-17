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

int GameObject::getX() const {
    return x;
};

int GameObject::getY() const {
    return y;
};

void GameObject::setX(int newX) {
    x = newX;
};

void GameObject::setY(int newY) {
    y = newY;
};

void GameObject::horizontalMovement(bool left) {};

bool GameObject::isStill() {
    return still;
};

void GameObject::setStill() {
    still = true;
};

int* GameObject::bottomRightCoordinates() {
    return 0;
};