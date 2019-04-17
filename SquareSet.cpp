#include "SquareSetRotationFunctions.h"
#include "objects.h"
#include <time.h>  

using namespace G;

bool SquareSet::instanceActive = false;

SquareSet::SquareSet(int startX, int startY):
    GameObject(startX, startY)
{

    SquareSet::instanceActive = true;

    std::srand(time(NULL));
    numberOfSquares = (std::rand() % 5) + 2;

    empty = true;
    squaresArray = new Square**[3];
    for (int i = 0; i < 3; i++) {
        squaresArray[i] = new Square*[3];
        for (int j = 0; j < 3; j++) {
            squaresArray[i][j] = nullptr;
        }
    }

};

bool SquareSet::isEmpty() {
    return empty;
};

void SquareSet::fill(int w, int h) {
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

Square*** SquareSet::getSquares() {
    return squaresArray;
};

void SquareSet::moveDown(int speed) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int oldY = squaresArray[i][j]->getY();
            squaresArray[i][j]->setY(oldY + speed);
        }
    }
    y = y + speed;
}

void SquareSet::horizontalMovement(bool left) {
    int squareWidth = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (squaresArray[i][j] != 0) {
                if (left)
                    squaresArray[i][j]->horizontalMovement(true);
                else 
                    squaresArray[i][j]->horizontalMovement(false);
                if (squareWidth == 0) squareWidth = squaresArray[i][j]->getWidth();
            }
        }
    }
    int change = left ? squareWidth * (-1) : squareWidth;
    setX(x + change);
};

bool SquareSet::nextToBorder(int w, bool left) {
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

void SquareSet::rotate() {

    Square*** tmpSquaresArray = new Square**[3];
    for (int i = 0; i < 3; i++) {
        tmpSquaresArray[i] = new Square*[3];
        for (int j = 0; j < 3; j++) {
            tmpSquaresArray[i][j] = nullptr;
        }
    }
    
    // srand(time(NULL));
    // int rotationType = rand() % 4 + 1;
    int rotationType = 3;
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

    for (int i = 0; i < 3; i++) {
        delete [] squaresArray[i];
    }
    delete [] squaresArray;

    squaresArray = tmpSquaresArray;

};

SquareSet::~SquareSet()
{

    SquareSet::instanceActive = false;
    // if (!isEmpty()) {
    //     delete square;
    // }

};