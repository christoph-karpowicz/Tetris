#include <vector>
#include <list>
#include <string>
#include <time.h>  
#include <emscripten.h>
#include <emscripten/bind.h>
#include "objects.h"

// Emscripten JS functions. 
EM_JS(void, drawSquare, (int x, int y, int w, int h), {
    const canvas = document.querySelector('canvas');
    const context = canvas.getContext('2d');
    context.beginPath();

    context.fillStyle = '#282828';
    context.fillRect(x, y, w, h);
    context.fill();

    context.fillStyle = '#ffffff';
    context.fillRect(x + 1, y + 1, w - 2, h - 2);
    context.fill();

    context.fillStyle = '#a5a5a5';
    context.fillRect(x + 4, y + 4, w - 8, h - 8);
    context.fill();
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

Game* Game::currentInstance = nullptr;

Game::Game(const int w, const int h): width(w), height(h), squareWidth(w/10), squareHeight(height/10) {
    Logger::add("Game loaded.");
    paused = false;
    squaresList = {};
    init();
    setStarted(false);
};

void Game::increaseGameSpeed() {
    int newSpeed = gameSpeed + 1;
    // Find next speed, that will match Game parameters.  
    while (newSpeed % gameSpeed != 0 || height % newSpeed != 0 || squareHeight % newSpeed != 0 || (height - squareHeight) % newSpeed != 0 || (height + (3 * squareHeight)) % newSpeed != 0) {
        newSpeed++;
    }
    setGameSpeed(newSpeed);
    Logger::add("Game speed increased to " + to_string(newSpeed) + ".");
};

void Game::setGameSpeed(const int speed) {
    gameSpeed = speed;
};

SquareSet* Game::getActiveSquareSet() const {
    return squareSet;
};

int Game::getGameSpeed() const {
    return gameSpeed;
};

emscripten::val Game::getGameToString() const {
    emscripten::val returnVal = emscripten::val::object();
    returnVal.set("parsedGame", emscripten::val(GameToStringParser::parseGame(Game::currentInstance)));
    return returnVal;
};

string Game::getLogs() const {
    // Consolidate logs from static Logger class. 
    string logs = "";
    for (string log : Logger::getLogs()) {
        logs.append(log);
        logs.append("||");
    }
    Logger::clear();
    return logs;
};

list<Square*> Game::getSquaresList() const {
    return squaresList;
};

bool Game::isGameOver() const {
    return gameOver;
};

void Game::setGameStatus(const bool over) {
    if (over) Logger::add("Game over.");
    gameOver = over;
};

void Game::init() {
    // Game initialization. 
    SquareSet* squareSet = nullptr;
    squaresList.clear();
    setGameStatus(false);
    setGameSpeed(2);
    setStarted(false);
    setScore(0);
    Logger::add("Game initialized.");
};

void Game::update() {

    Game::currentInstance = this;

    if (paused || !started) return;
        
    bool reset = false;
    clearCanvas();

    if (!isGameOver()) {

        if (!SquareSet::instanceActive) addSquareSet();

        // Checks if there any rows filled with squares.
        clearRows();
        
        // Checks if the active SquareSet reached the bottom.
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

        // Checks if the active SquareSet reached one of the static sqaures.
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

        // Transform active SquareSet into static squares.
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
            // Move SquareSet.
            (*squareSet) >> getGameSpeed();

            // Draw SquareSet.
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

    // Draw static squares.
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

    // Remove squares from squaresList.
    for (auto it = squaresList.begin(); it != squaresList.end(); ) {

        Square* sq = *it;
        if (sq->getY() == row * squareHeight) {
            it = squaresList.erase(it);
        } else {
            ++it;
        }

    }

    // Move squares from above down. 
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

    // Checks if any of the rows are filled with sqaures.
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

bool Game::getPaused() const {
    return paused;
};

int Game::getScore() const {
    return score;
};

bool Game::getStarted() const {
    return started;
};

int Game::getWidth() const {
    return width;
};

void Game::increaseScore() {
    int newScore = getScore() + 1;
    setScore(newScore);
    Logger::add("Row cleared. Score increased to " + to_string(newScore) + ".");
    if (getScore() % 1 == 0) {
        increaseGameSpeed();
    }
};

bool Game::isSquareSetBlocked() {

    bool blocked = false;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if ((*squareSet)[i][j] != 0) {

                for (Square* &s : squaresList) {

                    if ((s->getX() == (*squareSet)[i][j]->getX() + s->getWidth() || s->getX() == (*squareSet)[i][j]->getX() - s->getWidth()) && (*squareSet)[i][j]->getY() + (*squareSet)[i][j]->getHeight() >= s->getY() - getGameSpeed() && (*squareSet)[i][j]->getY() <= s->getY() + s->getHeight() + getGameSpeed()) {
                        blocked = true;
                    }
                    
                }

            }
        }
    }

    return blocked;

}

void Game::moveSquareSet(const bool left) {
    // Checks if squareSet is blocked by game border or other squares.
    if (!squareSet->isNextToBorder(width, left) && !isSquareSetBlocked())
        left ? ++(*squareSet) : (*squareSet)++;
};

void Game::setPaused(const bool val) {
    Logger::add(val ? "Game paused." : "Game unpaused.");
    paused = val;
};

void Game::reset() {
    Logger::add("Another try.");
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

void Game::setStarted(const bool val) {
    if (val) Logger::add("Game started.");
    started = val;
};

emscripten::val Game::getState() const {
    // Passes Game information to Javascript object. 
    emscripten::val returnVal = emscripten::val::object();
    returnVal.set("isGameOver", emscripten::val(isGameOver()));
    returnVal.set("score", emscripten::val(getScore()));
    returnVal.set("logs", emscripten::val(getLogs()));
    return returnVal;
};