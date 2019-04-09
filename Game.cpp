#include <vector>
#include <list>
#include <string>
#include <time.h>  
#include <emscripten.h>
#include <emscripten/bind.h>
#include "objects.h"

EM_JS(void, drawSquare, (int x, int y, int w, int h), {
    const canvas = document.querySelector('canvas');
    const context = canvas.getContext('2d');
    context.fillStyle = '#a5a5a5';
    context.beginPath();
    context.fillRect(x, y, w, h);
    // context.fillRect(50, 50, 50, 50);
    context.fill();
    // console.log(x, y, w, h);
});

EM_JS(void, clearCanvas, (), {
    const canvas = document.querySelector('canvas');
    const context = canvas.getContext('2d');
    context.clearRect(0, 0, canvas.width, canvas.height);
});

EM_JS(void, dump, (string data), {
    console.log(data);
});

using namespace G;
using namespace std;

Game::Game(int w, int h) {
    width = w;
    height = h;
    SquareSet* squareSet = nullptr;
    squaresList = {};
    gameOver = false;
};

void Game::init() {

};

void Game::update() {
    bool reset = false;
    clearCanvas();

    Square*** sq = squareSet->getSquares(); 

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sq[i][j] != 0) {
                if (sq[i][j]->getY() == height - sq[i][j]->getHeight()) {
                    reset = true;
                    goto resetConditionsMet;
                }
            }
        }
    }

    for (Square* &s : squaresList) {

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (sq[i][j] != 0) {
                    if (sq[i][j]->getY() + sq[i][j]->getHeight() == s->getY() && sq[i][j]->getX() == s->getX()) {
                        reset = true;
                        goto resetConditionsMet;
                    }
                }
            }
        }
        
    }

    resetConditionsMet:
    if (reset) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                sq[i][j]->setStill();
                squaresList.push_back(sq[i][j]);
            }
        }
        delete squareSet;

        for (Square* &s : squaresList) {
            int y = s->getY();
            bool sqaureStill = s->isStill();
            if (sqaureStill && y < 0) {
                gameOver = true;
                break;
            }
        }

        if (!gameOver) addSquareSet();
        
    }

    if (!gameOver) {
        squareSet->moveDown();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (sq[i][j] != 0) {
                    int x = sq[i][j]->getX();
                    int y = sq[i][j]->getY();
                    int w = sq[i][j]->getWidth();
                    int h = sq[i][j]->getHeight();
                    drawSquare(x, y, w, h);
                }
            }
        }
    }

    for (Square* &s : squaresList) {

        int x = s->getX();
        int y = s->getY();
        int w = s->getWidth();
        int h = s->getHeight();
        drawSquare(x, y, w, h);
        
    }

};

void Game::addSquareSet() {
    srand(time(NULL));
    int randX = rand() % (width/(width/10));
    // SquareSet newSquareSet = SquareSet(randX, 0);
    // SquareSet newSquareSet = SquareSet(0, 0);
    squareSet = new SquareSet(randX*(width/10), 0-(height/10 * 3));
    squareSet->fill(width/10,width/10);
    // squareSetsVector.push_back(newSquareSet);
};

void Game::deleteFigure() {

};

// emscripten::val Game::getSquareAtPostion(int position) {
//     // string squareDescription = "y: " + to_string(squareSetsVector.at(position).getY());
//     // return squareSetsVector.at(position);
//     emscripten::val returnVal = emscripten::val::object();
//     returnVal.set("square", emscripten::val(squareSetsVector.at(position).getYY()));
//     return returnVal;
// };
