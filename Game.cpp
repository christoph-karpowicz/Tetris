#include <vector>
#include <string>
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

// EM_JS(void, dump, (string data), {
//     console.log(data);
// });

using namespace G;
using namespace std;

Game::Game(int w, int h) {
    width = w;
    height = h;
    figuresVector = {};
    fcv = {};
};

void Game::init() {

};

void Game::update() {
    bool addAnotherPiece = true;
    // width += 1;  
    // dump(to_string(width));
    clearCanvas();
    for (Figure &f : figuresVector) {
        if (!f.isStill()) {
            
            int* coord;
            coord = f.bottomRightCoordinates();
            if (coord[1] != height) {

                // f.moveDown();

            } else {
                f.setStill();
            }
            
            addAnotherPiece = false;

        }

        Square*** figureSquares = f.getSquares();
        for (int i = 1; i <= f.getNumberOfSquares(); i++) {
            for (int j = 1; j <= f.getNumberOfSquares(); j++) {
                if (figureSquares[i][j] != 0) {
                    int x = figureSquares[i][j]->getX();
                    int y = figureSquares[i][j]->getY();
                    int w = figureSquares[i][j]->getWidth();
                    int h = figureSquares[i][j]->getHeight();
                    // if (x == NULL || x == 0) dump("aaaa");
                    // dump("aaaa");
                    drawSquare(x, y, w, h);
                }
            }
        }

        // for (int i = 0; i < f.getNumberOfSquares(); i++) {
        //     drawSquare(figureSquares[i]->getX(), figureSquares[i]->getY(), figureSquares[i]->getWidth(), figureSquares[i]->getHeight());
        //     // dump("aaa");
        // }
        // drawSquare(f.getX(), f.getY(), f.getWidth(), f.getHeight());
        // dump(to_string(s.getWidth()));
        
        
    }

    for (FigureCell &FC : fcv) {
        Square*** sq = FC.getSquares(); 
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

    if (addAnotherPiece) {
        // addFigure();
    }
};

void Game::addFigure() {
    Figure newFigure = Figure(0, 0, width/20, height/20);
    figuresVector.push_back(newFigure);
    FigureCell fc = FigureCell();
    fc.fill(50,50,50,50);
    fcv.push_back(fc);
};

void Game::deleteFigure() {

};

// emscripten::val Game::getSquareAtPostion(int position) {
//     // string squareDescription = "y: " + to_string(figuresVector.at(position).getY());
//     // return figuresVector.at(position);
//     emscripten::val returnVal = emscripten::val::object();
//     returnVal.set("square", emscripten::val(figuresVector.at(position).getYY()));
//     return returnVal;
// };
