#include "objects.h"

void G::verticallySymetric(Square*** tmpArray, SquareSet& squareSet) {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (squareSet.squaresArray[i][j] != 0) {
                if (i == 0) {
                    int newY = squareSet.getY() + (squareSet.squaresArray[i][j]->getHeight() * 2);
                    squareSet.squaresArray[i][j]->setY(newY);
                    tmpArray[2][j] = squareSet.squaresArray[i][j];
                }
                if (i == 1) {
                    tmpArray[i][j] = squareSet.squaresArray[i][j];
                }
                if (i == 2) {
                    int newY = squareSet.getY();
                    squareSet.squaresArray[i][j]->setY(newY);
                    tmpArray[0][j] = squareSet.squaresArray[i][j];
                }
            }
        }
    }

};

void G::horizontallySymetric(Square*** tmpArray, SquareSet& squareSet) {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (squareSet.squaresArray[i][j] != 0) {
                if (j == 0) {
                    int newX = squareSet.squaresArray[i][j]->getX() + (squareSet.squaresArray[i][j]->getWidth() * 2);
                    squareSet.squaresArray[i][j]->setX(newX);
                    tmpArray[i][2] = squareSet.squaresArray[i][j];
                }
                if (j == 1) {
                    tmpArray[i][j] = squareSet.squaresArray[i][j];
                }
                if (j == 2) {
                    int newX = squareSet.squaresArray[i][j]->getX() - (squareSet.squaresArray[i][j]->getWidth() * 2);
                    squareSet.squaresArray[i][j]->setX(newX);
                    tmpArray[i][0] = squareSet.squaresArray[i][j];
                }
            }
        }
    }

};

void G::deg90p(Square*** tmpArray, SquareSet& squareSet) {

    int k = 2;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (squareSet.squaresArray[i][j] != 0) {
                int newX = squareSet.getX() + (squareSet.squaresArray[i][j]->getWidth() * j);
                squareSet.squaresArray[i][j]->setX(newX);
                
                int newY = squareSet.getY() + (squareSet.squaresArray[i][j]->getHeight() * k);
                squareSet.squaresArray[i][j]->setY(newY);
                
                tmpArray[j][k] = squareSet.squaresArray[i][j];
            }
        }
        k--;
    }

};

void G::deg90m(Square*** tmpArray, SquareSet& squareSet) {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (squareSet.squaresArray[i][j] != 0) {
                int newX = squareSet.getX() + (squareSet.squaresArray[i][j]->getWidth() * j);
                squareSet.squaresArray[i][j]->setX(newX);
                
                int newY = squareSet.getY() + (squareSet.squaresArray[i][j]->getHeight() * i);
                squareSet.squaresArray[i][j]->setY(newY);
                
                tmpArray[j][i] = squareSet.squaresArray[i][j];
            }
        }
    }

};