#include "objects.h"

void G::verticallySymetric(Square*** tmpArray, SquareSet& squareSet) {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (squareSet.squaresArray[i][j] != 0) {
                if (i == 0) {
                    int newY = squareSet.getY() + (squareSet.squaresArray[i][j]->getHeight() * 2);
                    tmpArray[2][j] = new Square(squareSet.squaresArray[i][j]->getX(), newY, squareSet.squaresArray[i][j]->getWidth(), squareSet.squaresArray[i][j]->getHeight());
                }
                if (i == 1) {
                    tmpArray[i][j] = new Square(squareSet.squaresArray[i][j]->getX(), squareSet.squaresArray[i][j]->getY(), squareSet.squaresArray[i][j]->getWidth(), squareSet.squaresArray[i][j]->getHeight());
                }
                if (i == 2) {
                    int newY = squareSet.getY();
                    tmpArray[0][j] = new Square(squareSet.squaresArray[i][j]->getX(), newY, squareSet.squaresArray[i][j]->getWidth(), squareSet.squaresArray[i][j]->getHeight());
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
                    tmpArray[i][2] = new Square(newX, squareSet.squaresArray[i][j]->getY(), squareSet.squaresArray[i][j]->getWidth(), squareSet.squaresArray[i][j]->getHeight());
                }
                if (j == 1) {
                    tmpArray[i][j] = new Square(squareSet.squaresArray[i][j]->getX(), squareSet.squaresArray[i][j]->getY(), squareSet.squaresArray[i][j]->getWidth(), squareSet.squaresArray[i][j]->getHeight());
                }
                if (j == 2) {
                    int newX = squareSet.squaresArray[i][j]->getX() - (squareSet.squaresArray[i][j]->getWidth() * 2);
                    tmpArray[i][0] = new Square(newX, squareSet.squaresArray[i][j]->getY(), squareSet.squaresArray[i][j]->getWidth(), squareSet.squaresArray[i][j]->getHeight());
                }
            }
        }
    }

};

void G::deg90p(Square*** tmpArray, SquareSet& squareSet) {

    register int k = 2;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (squareSet.squaresArray[i][j] != 0) {
                int newX = squareSet.getX() + (squareSet.squaresArray[i][j]->getWidth() * k);
                
                int newY = squareSet.getY() + (squareSet.squaresArray[i][j]->getHeight() * j);
                
                tmpArray[j][k] = new Square(newX, newY, squareSet.squaresArray[i][j]->getWidth(), squareSet.squaresArray[i][j]->getHeight());
            }
        }
        k--;
    }

};

void G::deg90m(Square*** tmpArray, SquareSet& squareSet) {

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            if (squareSet.squaresArray[i][j] != 0) {
                int newX = squareSet.getX() + (squareSet.squaresArray[i][j]->getWidth() * i);
                
                int newY = squareSet.getY() + (squareSet.squaresArray[i][j]->getHeight() * j);
                
                tmpArray[j][i] = new Square(newX, newY, squareSet.squaresArray[i][j]->getWidth(), squareSet.squaresArray[i][j]->getHeight());
            }
        }
    }

};