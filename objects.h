#ifndef OBJECTS_H
#define OBJECTS_H

#include <emscripten.h>
#include <emscripten/bind.h>
#include <vector>
#include <list>
#include <string>

using namespace std;

namespace G {

    class GameObject {
        protected: 
            int x, y;
            bool still;
            
        public:
            GameObject(int startX, int startY);
            virtual void moveDown();
            int getX() const;
            int getY() const;
            void setX(int newX);
            void setY(int newY);
            virtual void horizontalMovement(bool left);
            bool isStill();
            void setStill();
            virtual int* bottomRightCoordinates();
            
    };

    class Square: public GameObject {
        private:
            int width, height;
            bool active;
        
        public:
            Square(int startX, int startY, int w, int h);
            emscripten::val getYY() const;
            int getWidth() const;
            int getHeight() const;
            int* bottomRightCoordinates();
            void horizontalMovement(bool left);

    };

    class SquareSet: public GameObject {
        private:
            Square*** squaresArray;
            int numberOfSquares;
            bool corrected;
                    
        public:
            static bool instanceActive;
            SquareSet(int startX, int startY);
            ~SquareSet();
            void fill(int w, int h);
            Square*** getSquares();
            void moveDown(int speed);
            void horizontalMovement(bool left);
            bool nextToBorder(int w, bool left);
            void rotate();
            friend void verticallySymetric(Square*** tmpArray, SquareSet& squareSet);
            friend void horizontallySymetric(Square*** tmpArray, SquareSet& squareSet);
            friend void deg90p(Square*** tmpArray, SquareSet& squareSet);
            friend void deg90m(Square*** tmpArray, SquareSet& squareSet);

    };

    class Game {
        private:
            bool gameOver;
            int width, height, squareWidth, squareHeight, score, gameSpeed;
            SquareSet* squareSet;
            list<Square*> squaresList;
            void clearRow(int row, int perRow);
            void clearRows();

        public:
            Game(int w, int h);
            void init();
            void update();
            void addSquareSet();
            void deleteSquareSet();
            int getGameSpeed() const;
            void increaseGameSpeed();
            void setGameSpeed(int speed);
            bool isGameOver() const;
            void setGameStatus(bool over);
            void moveSquareSet(bool left);
            void rotateSquareSet();
            int getScore() const;
            void setScore(int newScore);
            void increaseScore();
            // emscripten::val getSquareAtPostion(int position);
    };

    void verticallySymetric(Square*** tmpArray, SquareSet& squareSet);
    void horizontallySymetric(Square*** tmpArray, SquareSet& squareSet);
    void deg90p(Square*** tmpArray, SquareSet& squareSet);
    void deg90m(Square*** tmpArray, SquareSet& squareSet);

}

#endif