#include "objects.h"
#include <time.h>  

using namespace G;

bool SquareSet::instanceActive = false;

SquareSet::SquareSet(const int startX, const int startY):
    GameObject(startX, startY)
{

    SquareSet::instanceActive = true;

    std::srand(time(NULL));
    numberOfSquares = (std::rand() % 5) + 2;

    squaresArray = new Square**[3];
    for (int i = 0; i < 3; i++) {
        squaresArray[i] = new Square*[3];
        for (int j = 0; j < 3; j++) {
            squaresArray[i][j] = nullptr;
        }
    }

    corrected = false;

};

bool SquareSet::isBeyondBorder(const int w, Square*** tmpArray) const {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tmpArray[i][j] != 0) {
                if (tmpArray[i][j]->getX() < 0 || tmpArray[i][j]->getX() >= w - tmpArray[i][j]->getWidth()) {
                    return true;
                    break;
                }
            }
        }
    }
    return false;
};

bool SquareSet::isNextToBorder(const int w, const bool left) const {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (squaresArray[i][j] != 0) {
                if ((squaresArray[i][j]->getX() == 0 && left) || (squaresArray[i][j]->getX() == w - squaresArray[i][j]->getWidth() && !left)) {
                    return true;
                    break;
                }
            }
        }
    }
    return false;
};

void SquareSet::fill(const int w, const int h) {
    int squaresCreated = 0;
    int randRow, randCol;

    while (squaresCreated != numberOfSquares) {
        
        if (squaresCreated == 0) {
            squaresArray[1][1] = new Square(x + w, y + h, w, h);
            squaresCreated++;
        }
        else {
            int allowed = false;
            randRow = rand() % 3;
            randCol = rand() % 3;
            for (int r = 0; r < 3; r++) {
                if ((r == randRow - 1 || r == randRow + 1) && squaresArray[r][randCol] != 0) {
                    allowed = true;
                }
            }
            if (!allowed)
                for (int k = 0; k < 3; k++) {
                    if ((k == randCol - 1 || k == randCol + 1) && squaresArray[randRow][k] != 0) {
                        allowed = true;
                    }
                }
            if (allowed) {
                squaresArray[randRow][randCol] = new Square(x + randCol*w, y + randRow*h, w, h);
                squaresCreated++;
            }
        }
        
    }
};

Square*** SquareSet::getSquares() const {
    return squaresArray;
};

SquareSet& SquareSet::operator>>(const int speed) {

    if (!corrected) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (squaresArray[i][j] != 0) {
                    while (squaresArray[i][j]->getY() % speed != 0) {
                        squaresArray[i][j]->setY(squaresArray[i][j]->getY() + 1);
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (squaresArray[i][j] != 0) {
                int oldY = squaresArray[i][j]->getY();
                squaresArray[i][j]->setY(oldY + speed);
            }
        }
    }
    // correct y aswell?
    y = y + speed;

    return *this;

}

int* SquareSet::bottomRightCoordinates() const {
    return nullptr;
}

void SquareSet::horizontalMovement(const bool left) {
    int squareWidth = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (squaresArray[i][j] != 0) {
                if (left) {
                    // Move left.
                    ++(*squaresArray[i][j]);
                }
                else 
                    // Move right.
                    (*squaresArray[i][j])++;
                if (squareWidth == 0) squareWidth = squaresArray[i][j]->getWidth();
            }
        }
    }
    int change = left ? squareWidth * (-1) : squareWidth;
    setX(x + change);
};

void SquareSet::rotate() {

    Square*** tmpSquaresArray = new Square**[3];
    for (int i = 0; i < 3; i++) {
        tmpSquaresArray[i] = new Square*[3];
        for (int j = 0; j < 3; j++) {
            tmpSquaresArray[i][j] = nullptr;
        }
    }
    
    // Perform a random rotation. 
    srand(time(NULL));
    int rotationType = rand() % 4 + 1;
    switch (rotationType) {
        case 1:
            verticallySymetric(tmpSquaresArray, *this);
            break;
        case 2:
            horizontallySymetric(tmpSquaresArray, *this);
            break;
        case 3:
            deg90p(tmpSquaresArray, *this);
            break;
        case 4:
            deg90m(tmpSquaresArray, *this);
            break;
    }

    // Checks if any of the squares have end up outside the border or inside another square.
    if (validateRotation(Game::currentInstance->getSquaresList(), tmpSquaresArray)) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                delete squaresArray[i][j];
            }
            delete [] squaresArray[i];
        }
        delete [] squaresArray;

        squaresArray = tmpSquaresArray;
    } 

};

bool SquareSet::validateRotation(list<Square*> squaresList, Square*** tmpArray) {
    
    // Is beyond the border.
    if (isBeyondBorder(Game::currentInstance->getWidth(), tmpArray)) {
        return false;
    }

    bool valid = true;

    // Is on another square. 
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tmpArray[i][j] != 0) {

                for (Square* &s : squaresList) {

                    if ((s->getX() == tmpArray[i][j]->getX() + s->getWidth() || s->getX() == tmpArray[i][j]->getX() - s->getWidth()) && tmpArray[i][j]->getY() + tmpArray[i][j]->getHeight() >= s->getY() && tmpArray[i][j]->getY() <= s->getY() + s->getHeight()) {
                        valid = false;
                    }
                    
                }

            }
        }
    }

    return valid;
    
};

SquareSet& SquareSet::operator++() {
    horizontalMovement(true);
    return *this;
};

SquareSet& SquareSet::operator++(int) {
    horizontalMovement(false);
    return *this;
};

Square** SquareSet::operator[](const int row) const {
    return squaresArray[row];
};

SquareSet::~SquareSet()
{

    SquareSet::instanceActive = false;

};