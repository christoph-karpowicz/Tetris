#include <emscripten.h>
#include <emscripten/bind.h>

#include "objects.h"

EM_JS(void, dump, (string data), {
    console.log(data);
});

using namespace G;

Figure::Figure(int startX, int startY, int w, int h):
    GameObject(startX, startY)
{

    arrayDim = 3;
    // Square ***squaresArray;
    squaresArray = new Square**[arrayDim];

    for (int i = 0; i < arrayDim; i++) {
        squaresArray[i] = new Square*[arrayDim];
        for (int j = 0; j < arrayDim; j++) {
            squaresArray[i][j] = nullptr;
        }
    }
    // vector<vector<FigureCell>> squaresVector = {};
    // for (int i = 0; i < arrayDim; i++) {
    //     vector<FigureCell> newVector = {};
    //     for (int j = 0; j < arrayDim; j++) {
    //         FigureCell newFigureCell = FigureCell();
    //         newVector.push_back(newFigureCell);
    //     }
    //     squaresVector.push_back(newVector);
    // }

    // numberOfSquares = rand() % 5 + 1;
    numberOfSquares = 2;
    int squaresGenerated = 0;
    int gameWidth = w;
    int gameHeight = h;
    int squareWidth = gameWidth/20;
    int squareHeight = gameHeight/20;

    addSquare(squareWidth, squareHeight, squareWidth, squareHeight, 1, 2);
    addSquare(squareWidth, squareHeight, squareWidth, squareHeight, 2, 1);

    int randomRow, randomColumn, actualTop, actualLeft;
    bool add;

    // while(squaresGenerated != numberOfSquares) {
    //     randomRow = rand() % 3;
    //     randomColumn = rand() % 3;
    //     actualTop = randomRow * squareHeight;
    //     actualLeft = randomColumn * squareWidth;
    //     add = false;

    //     // for (Square &s : squaresVector) {
                
    //         // if (s.getX() != actualLeft && s.getY() != actualTop && (actualLeft == s.getX() + squareWidth || actualLeft == s.getX() - squareWidth) && (actualTop == s.getY() + squareHeight || actualTop == s.getY() - squareHeight)) {
    //             // && (squaresArray[randomRow - 1][randomColumn] )
    //         if (squaresArray[randomRow][randomColumn] == 0) {

    //             add = true;

    //         }
        
    //     // }

    //     if (add) {
    //         addSquare(actualLeft, actualTop, squareWidth, squareHeight, randomRow, randomColumn);
    //         squaresGenerated++;
    //     }

    // }

    // addSquare(squareWidth, squareHeight, squareWidth, squareHeight, 2, 1);

};

void Figure::addSquare(int startX, int startY, int w, int h, int row, int column) {
    // vector<FigureCell> &columnVector = squaresVector[row];
    // FigureCell &cell = columnVector[column];
    // cell.fill(startX, startY, w, h);
    // Square newSquare = Square(startX, startY, w, h);
    // squaresVector.at(row).at(column) = newSquare;
    squaresArray[row][column] = new Square(startX, startY, w, h);
};

// vector<vector<FigureCell>>* Figure::getSquares() {
// Square** Figure::getSquares() {
Square*** Figure::getSquares() {
    // Square** squareReturnArray;
    // squareReturnArray = new Square*[numberOfSquares];
    // int currentIndex;
    // currentIndex = 0;

    // for (int i = 1; i <= arrayDim; i++) {
    //     for (int j = 1; j <= arrayDim; j++) {
    //         if (squaresArray[i][j] != 0) {
    //             squareReturnArray[currentIndex] = squaresArray[i][j];
    //             // currentIndex = currentIndex + 1;
    //         }
    //     }
    // }

    return squaresArray;
    // return squareReturnArray;
    // return &squaresVector;
};

int Figure::getNumberOfSquares() const {
    return numberOfSquares;
}

int* Figure::bottomRightCoordinates() {

    int coordinates[2];

    // for (int i = arrayDim - 1; i >= 0; i--) {
    //     for (int j = 0; j < arrayDim; j++) {
    //         if (squaresArray[i][j] != 0) {
    //             coordinates[0] = squaresArray[i][j]->getX() + squaresArray[i][j]->getWidth();
    //             coordinates[1] = squaresArray[i][j]->getY() + squaresArray[i][j]->getHeight();
    //             break;
    //         }
    //     }
    // }

    // for (int i = arrayDim; i > 0; i--) {
    //     vector<FigureCell> &columnVector = squaresVector[i];
    //     for (int j = 0; j < arrayDim; j++) {
    //         FigureCell &cell = columnVector[j];
    //         bool isEmpty = cell.isEmpty();
    //         if (!cell.isEmpty()) {
    //             coordinates[0] = cell.getSquare()->getX() + cell.getSquare()->getWidth();
    //             coordinates[1] = cell.getSquare()->getY() + cell.getSquare()->getHeight();
    //             break;
    //         }
    //     }
    // }
    for (int i = arrayDim; i >= 1; i--) {
        for (int j = 1; j <= arrayDim; j++) {
            if (!squaresVector[i][j].isEmpty()) {
                // coordinates[0] = squaresVector[i][j].getSquare()->getX() + squaresVector[i][j].getSquare()->getWidth();
                // coordinates[1] = squaresVector[i][j].getSquare()->getY() + squaresVector[i][j].getSquare()->getHeight();
                break;
            }
        }
    }

    // coordinates[0] = 1;
    // coordinates[1] = 2;

    return coordinates;
}

Figure::~Figure() {
    // for (int i = 1; i <= arrayDim + 1; i++) {
    //     for (int j = 1; j <= arrayDim + 1; j++) {
    //         // if (squaresArray[i][j] != 0)
    //             delete squaresArray[i][j];
    //     }
    //     delete [] squaresArray[i];
    // }
    // dump("aaaa");
    // delete [] squaresArray;
}