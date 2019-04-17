#include "objects.h"

using namespace G;

Square::Square(int startX, int startY, int w, int h): 
    GameObject(startX, startY),
    width(w),
    height(h) 
{
    active = false;
};

emscripten::val Square::getYY() const {
    emscripten::val returnVal = emscripten::val::object();
    returnVal.set("y", emscripten::val(y));
    return returnVal;
}

void Square::horizontalMovement(bool left) {
    int change = left ? width * (-1) : width;
    setX(x + change);
};

int Square::getWidth() const {
    return width;
};

int Square::getHeight() const {
    return height;
};

int* Square::bottomRightCoordinates() {
    int coordinates[2] = {x + width, y + height};
    return coordinates;
}