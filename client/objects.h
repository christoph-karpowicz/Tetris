#ifndef OBJECTS_H
#define OBJECTS_H

#include <emscripten.h>
#include <emscripten/bind.h>
#include <vector>
#include <list>
#include <string>

using namespace std;

namespace G {

    class Shape {
        protected:
            const int width, height;
            virtual int getWidth() const = 0;
            virtual int getHeight() const = 0;
        public:
            Shape(const int w, const int h);

    };

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
            virtual void horizontalMovement(const bool left) = 0;
            bool isStill() const;
            void setStill();
            virtual int* bottomRightCoordinates() const = 0;
            
    };

    class Square: public GameObject, private Shape {
        
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
            bool validateRotation(list<Square*> squaresList, Square*** tmpArray);
                    
        public:
            static bool instanceActive;
            SquareSet(const int startX, const int startY);
            ~SquareSet();
            bool isBeyondBorder(const int w, Square*** tmpArray) const;
            bool isNextToBorder(const int w, const bool left) const;
            void fill(const int w, const int h);
            Square*** getSquares() const;
            int* bottomRightCoordinates() const;
            void horizontalMovement(const bool left);
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

    class Logger {
        private:
            static vector<string> queue;
        public:
            static void add(string log);
            static void clear();
            static vector<string> getLogs();
    };

    class Game {
        private:
            bool gameOver, started, paused;
            const int width, height, squareWidth, squareHeight;
            int score, gameSpeed;
            mutable SquareSet* squareSet;
            list<Square*> squaresList;
            void clearRow(const int row, const int perRow);
            void clearRows();
            void init();
            void addSquareSet() const;
            void deleteSquareSet() const;
            string getLogs() const;
            void increaseGameSpeed();
            void setGameSpeed(const int speed);
            void setGameStatus(const bool over);
            void increaseScore();
            bool isSquareSetBlocked();
            void setScore(const int newScore);
            bool isGameOver() const;
            
        public:
            Game(const int w, const int h);
            static Game* currentInstance;
            SquareSet* getActiveSquareSet() const;
            int getGameSpeed() const;
            emscripten::val getGameToString() const;
            bool getPaused() const;
            int getScore() const;
            list<Square*> getSquaresList() const;
            bool getStarted() const;
            emscripten::val getState() const;
            int getWidth() const;
            void moveSquareSet(const bool left);
            void reset();
            void rotateSquareSet() const;
            void setPaused(const bool val);
            void setStarted(const bool val);
            void update();
    };

    class GameToStringParser {
        private:
            static GameToStringParser parser;
            string parse(Game &game);
        public:
            static string parseGame(Game* game);
    };

    void verticallySymetric(Square*** tmpArray, SquareSet& squareSet);
    void horizontallySymetric(Square*** tmpArray, SquareSet& squareSet);
    void deg90p(Square*** tmpArray, SquareSet& squareSet);
    void deg90m(Square*** tmpArray, SquareSet& squareSet);

}

#endif