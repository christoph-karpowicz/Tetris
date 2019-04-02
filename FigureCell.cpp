#include "objects.h"

using namespace G;

FigureCell::FigureCell()
{

    empty = true;
    sa = new Square**[3];
    for (int i = 0; i < 3; i++) {
        sa[i] = new Square*[3];
        for (int j = 0; j < 3; j++) {
            sa[i][j] = nullptr;
        }
    }

};

bool FigureCell::isEmpty() {
    return empty;
};

void FigureCell::fill(int startX, int startY, int w, int h) {
    // for (int i = 0; i < 3; i++) {
    //     for (int j = 0; j < 3; j++) {
    //         sa[i][j] = new Square(i*50, 0, 50, 50);
    //     }
    // }
    sa[1][2] = new Square(1*50, 0, 50, 50);
    // square = new Square(startX, startY, w, h);
    // empty = false;
};

// Square* FigureCell::getSquare() {
//     return square;
// };

Square*** FigureCell::getSquares() {
    return sa;
};

FigureCell::~FigureCell()
{

    // if (!isEmpty()) {
    //     delete square;
    // }

};