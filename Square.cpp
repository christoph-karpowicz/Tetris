#include "objects.h"

using namespace G;

Square::Square(int startX, int startY, int w, int h): 
    GameObject(startX, startY),
    width(w),
    height(h) 
{

};

int Square::getX() const {
    return x;
};

int Square::getY() const {
    return y;
};

emscripten::val Square::getYY() const {
    emscripten::val returnVal = emscripten::val::object();
    returnVal.set("y", emscripten::val(y));
    return returnVal;
}

void Square::setX(int newX) {
    x = newX;
};

void Square::setY(int newY) {
    y = newY;
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