#include "objects.h"

using namespace G;

Square::Square(const int startX, const int startY, const int w, const int h): 
    GameObject(startX, startY),
    Shape(w, h)
{};

void Square::horizontalMovement(const bool left) {
    int change = left ? width * (-1) : width;
    setX(x + change);
};

int Square::getWidth() const {
    return width;
};

int Square::getHeight() const {
    return height;
};

int* Square::bottomRightCoordinates() const {
    int coordinates[2] = {x + width, y + height};
    return coordinates;
}

Square& Square::operator++() {
    horizontalMovement(true);
    return *this;
};

Square& Square::operator++(int) {
    horizontalMovement(false);
    return *this;
};