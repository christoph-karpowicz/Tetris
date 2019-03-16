#include <emscripten.h>
#include <emscripten/bind.h>
#include "objects.h"

using namespace G;

Shape::Shape(int startX, int startY, int w, int h): 
    x(startX),
    y(startY),
    width(w),
    height(h)
{
    still = false;
};

void Shape::moveDown() {
    setY(y + 1);
    // y = 22;
};

int Shape::getX() const {
    return x;
};

int Shape::getY() const {
    return y;
};

emscripten::val Shape::getYY() {
    emscripten::val returnVal = emscripten::val::object();
    returnVal.set("y", emscripten::val(y));
    return returnVal;
}

void Shape::setX(int newX) {
    x = newX;
};

void Shape::setY(int newY) {
    y = newY;
};

int Shape::getWidth() {
    return width;
};

int Shape::getHeight() {
    return height;
};

bool Shape::isStill() {
    return still;
};