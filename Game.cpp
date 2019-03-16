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
    vector<Square> squaresVector = {};
};

void Game::init() {

};

void Game::update() {
    // width += 1;  
    // dump(to_string(width));
    clearCanvas();
    for (Square s : squaresVector) {
        if (!s.isStill()) {
            s.moveDown();
        }
        drawSquare(s.getX(), s.getY(), s.getWidth(), s.getHeight());
        // dump(to_string(s.getWidth()));
    }
};

void Game::addSquare() {
    Square newSquare = Square(0, 0, width/10, height/10);
    squaresVector.push_back(newSquare);
};

void Game::deleteSquare() {

};

emscripten::val Game::getSquareAtPostion(int position) {
    // string squareDescription = "y: " + to_string(squaresVector.at(position).getY());
    // return squaresVector.at(position);
    emscripten::val returnVal = emscripten::val::object();
    returnVal.set("square", emscripten::val(squaresVector.at(position).getY()));
    return returnVal;
};
