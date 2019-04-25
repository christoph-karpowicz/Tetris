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

Game::Game(const int w, const int h): width(w), height(h), squareWidth(w/10), squareHeight(height/10) {
    squaresList = {};
    init();
    setIsStopped(true);
};

void Game::increaseGameSpeed() {
    int newSpeed = gameSpeed + 1;
    while (newSpeed % gameSpeed != 0 || height % newSpeed != 0 || squareHeight % newSpeed != 0 || (height - squareHeight) % newSpeed != 0 || (height + (3 * squareHeight)) % newSpeed != 0) {
        newSpeed++;
    }
    setGameSpeed(newSpeed);
};

void Game::setGameSpeed(const int speed) {
    gameSpeed = speed;
};

int Game::getGameSpeed() const {
    return gameSpeed;
};

bool Game::isGameOver() const {
    return gameOver;
};

void Game::setGameStatus(const bool over) {
    gameOver = over;
};

void Game::init() {
    SquareSet* squareSet = nullptr;
    squaresList.clear();
    setGameStatus(false);
    setGameSpeed(2);
    setScore(0);
};

void Game::update() {

    if (stopped) return;
        
    bool reset = false;
    clearCanvas();

    if (!isGameOver()) {

        if (!SquareSet::instanceActive) addSquareSet();

        clearRows();
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if ((*squareSet)[i][j] != 0) {
                    int bottom = height - (*squareSet)[i][j]->getHeight();
                    if ((*squareSet)[i][j]->getY() == bottom) {
                        reset = true;
                        goto resetConditionsMet;
                    }
                }
            }
        }

        for (Square* &s : squaresList) {

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if ((*squareSet)[i][j] != 0) {
                        if ((*squareSet)[i][j]->getY() + (*squareSet)[i][j]->getHeight() == s->getY() && (*squareSet)[i][j]->getX() == s->getX()) {
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
                    if ((*squareSet)[i][j] != 0) {
                        (*squareSet)[i][j]->setStill();
                        squaresList.push_back((*squareSet)[i][j]);
                    }
                }
            }

            for (Square* &s : squaresList) {
                int y = s->getY();
                bool sqaureStill = s->isStill();
                if (sqaureStill && y < 0) {
                    setGameStatus(true);
                    break;
                }
            }

            deleteSquareSet();

        }

        if (!isGameOver()) {
            (*squareSet) >> getGameSpeed();

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if ((*squareSet)[i][j] != 0) {
                        int x = (*squareSet)[i][j]->getX();
                        int y = (*squareSet)[i][j]->getY();
                        int w = (*squareSet)[i][j]->getWidth();
                        int h = (*squareSet)[i][j]->getHeight();
                        drawSquare(x, y, w, h);
                    }
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

void Game::addSquareSet() const {

    if (!SquareSet::instanceActive) {

        srand(time(NULL));
        int randX = rand() % (width/squareWidth - 2);
        squareSet = new SquareSet(randX*(squareWidth), 0 - (squareHeight * 3));
        squareSet->fill(squareWidth, squareHeight);

    }
    
};

void Game::clearRow(const int row, const int perRow) {

    for (auto it = squaresList.begin(); it != squaresList.end(); ) {

        Square* sq = *it;
        if (sq->getY() == row * squareHeight) {
            it = squaresList.erase(it);
        } else {
            ++it;
        }

    }

    for (Square* &s : squaresList) {

        if (s->getY() < row * squareHeight) {
            int newY = s->getY() + squareHeight;
            s->setY(newY);
        }
        
    }

    increaseScore();

};

void Game::clearRows() {

    int squaresPerRow = width/squareWidth,
        rows = height/squareHeight;

    for (int i = 0; i < rows; i++) {

        int squaresFound = 0;

        for (Square* &s : squaresList) {

            if (s->getY() == i * squareHeight) {
                squaresFound++;
            } 
            if (squaresFound == squaresPerRow) {
                clearRow(i, squaresPerRow);
                i = 0;
                break;
            }
            
        }

    }

};

int Game::getScore() const {
    return score;
};

void Game::increaseScore() {
    setScore(getScore() + 1);
    if (getScore() % 1 == 0) {
        increaseGameSpeed();
    }
};

void Game::moveSquareSet(const bool left) {
    if (!squareSet->nextToBorder(width, left))
        left ? ++(*squareSet) : (*squareSet)++;
};

void Game::reset() {
    init();
};

void Game::rotateSquareSet() const {
    squareSet->rotate();
};

void Game::deleteSquareSet() const {
    delete squareSet;
    if (!isGameOver()) addSquareSet();
};

void Game::setScore(const int newScore) {
    score = newScore;
};

void Game::setIsStopped(const bool val) {
    stopped = val;
};

emscripten::val Game::getState() const {
    emscripten::val returnVal = emscripten::val::object();
    returnVal.set("isGameOver", emscripten::val(isGameOver()));
    returnVal.set("score", emscripten::val(getScore()));
    return returnVal;
};

// emscripten::val Game::getSquareAtPostion(int position) {
//     // string squareDescription = "y: " + to_string(squareSetsVector.at(position).getY());
//     // return squareSetsVector.at(position);
//     emscripten::val returnVal = emscripten::val::object();
//     returnVal.set("square", emscripten::val(squareSetsVector.at(position).getYY()));
//     return returnVal;
// };