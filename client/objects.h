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
            GameObject(const int startX, const int startY);
            virtual void moveDown();
            int getX() const;
            int getY() const;
            void setX(int newX);
            void setY(int newY);
            virtual void horizontalMovement(const bool left);
            bool isStill() const;
            void setStill();
            virtual int* bottomRightCoordinates() const;
            
    };

    class Square: public GameObject {
        private:
            const int width, height;
            bool active;
        
        public:
            Square(const int startX, const int startY, const int w, const int h);
            int getWidth() const;
            int getHeight() const;
            int* bottomRightCoordinates() const;
            void horizontalMovement(const bool left);
            Square& operator++();
            Square& operator++(int);

    };

    class SquareSet: public GameObject {
        private:
            Square*** squaresArray;
            int numberOfSquares;
            bool corrected;
                    
        public:
            static bool instanceActive;
            SquareSet(const int startX, const int startY);
            ~SquareSet();
            void fill(const int w, const int h);
            Square*** getSquares() const;
            void horizontalMovement(const bool left);
            bool nextToBorder(const int w, const bool left) const;
            void rotate();

            SquareSet& operator++();
            SquareSet& operator++(int);
            Square** operator[](const int row) const;
            SquareSet& operator>>(const int speed);

            friend void verticallySymetric(Square*** tmpArray, SquareSet& squareSet);
            friend void horizontallySymetric(Square*** tmpArray, SquareSet& squareSet);
            friend void deg90p(Square*** tmpArray, SquareSet& squareSet);
            friend void deg90m(Square*** tmpArray, SquareSet& squareSet);

    };

    class Game {
        private:
            bool gameOver, stopped;
            const int width, height, squareWidth, squareHeight;
            int score, gameSpeed;
            mutable SquareSet* squareSet;
            list<Square*> squaresList;
            void clearRow(const int row, const int perRow);
            void clearRows();
            void init();
            void addSquareSet() const;
            void deleteSquareSet() const;
            int getGameSpeed() const;
            void increaseGameSpeed();
            void setGameSpeed(const int speed);
            void setGameStatus(const bool over);
            void increaseScore();
            int getScore() const;
            void setScore(const int newScore);
            bool isGameOver() const;
        public:
            Game(const int w, const int h);
            emscripten::val getState() const;
            void update();
            void moveSquareSet(const bool left);
            void rotateSquareSet() const;
            void reset();
            void setIsStopped(const bool val);
    };

    void verticallySymetric(Square*** tmpArray, SquareSet& squareSet);
    void horizontallySymetric(Square*** tmpArray, SquareSet& squareSet);
    void deg90p(Square*** tmpArray, SquareSet& squareSet);
    void deg90m(Square*** tmpArray, SquareSet& squareSet);

}

#endif