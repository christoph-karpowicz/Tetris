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
    sa = new Square**[3];
    for (int i = 0; i < 3; i++) {
        sa[i] = new Square*[3];
        for (int j = 0; j < 3; j++) {
            sa[i][j] = nullptr;
        }
    }

};

bool SquareSet::isEmpty() {
    return empty;
};

void SquareSet::fill(int w, int h) {
    int squaresCreated = 0;
    int randRow, randCol;
    // bool rowAllowed, columnAllowed;
    // vector<int> allowedRows = {1};
    // vector<int> allowedColumns = {1};

    while (squaresCreated != numberOfSquares) {
        
        if (squaresCreated == 0) {
            sa[1][1] = new Square(x + w, y + h, w, h);
            squaresCreated++;
        }
        else {
            int allowed = false;
            // std::srand(time(NULL));
            randRow = rand() % 3;
            // std::srand(time(NULL));
            randCol = rand() % 3;
            for (int r = 0; r < 3; r++) {
                if ((r == randRow - 1 || r == randRow + 1) && sa[r][randCol] != 0) {
                    allowed = true;
                }
            }
            if (!allowed)
                for (int k = 0; k < 3; k++) {
                    if ((k == randCol - 1 || k == randCol + 1) && sa[randRow][k] != 0) {
                        allowed = true;
                    }
                }
            if (allowed) {
                sa[randCol][randRow] = new Square(x + randRow*w, y + randCol*h, w, h);
                squaresCreated++;
            }
            // for (int val : allowedRows) {
            //     if (val == randRow) rowAllowed = true; 
            // }
            // for (int val : allowedColumns) {
            //     if (val == randCol) columnAllowed = true; 
            // }
            // if (sa[randCol][randRow] == 0 && (rowAllowed || columnAllowed)) {
            //     sa[randCol][randRow] = new Square(x + randRow*w, y + randCol*h, w, h);
            //     if (!rowAllowed) allowedRows.push_back(randRow);
            //     if (!columnAllowed) allowedColumns.push_back(randCol);
            //     squaresCreated++;
            // }
        }
        
        // bool done = false;
        // for (int i = 0; i < 3; i++) {
        //     for (int j = 0; j < 3; j++) {
        //         if (sa[i][j] == 0) {
        //             sa[i][j] = new Square(j*50, i*50, 50, 50);
        //             done = true;
        //             squaresCreated++;
        //             break;
        //         }
        //         if (done) break;
        //     }
        // }
    }
    // sa[1][2] = new Square(1*50, 0, 50, 50);
    // square = new Square(startX, startY, w, h);
    // empty = false;
};

// Square* SquareSet::getSquare() {
//     return square;
// };

Square*** SquareSet::getSquares() {
    return sa;
};

void SquareSet::moveDown() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int oldY = sa[i][j]->getY();
            sa[i][j]->setY(oldY + 2);
        }
    }
}

SquareSet::~SquareSet()
{

    SquareSet::instanceActive = false;
    // if (!isEmpty()) {
    //     delete square;
    // }

};