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
            int getX() const;
            int getY() const;
            emscripten::val getYY() const;
            void setX(int newX);
            void setY(int newY);
            int getWidth() const;
            int getHeight() const;
            int* bottomRightCoordinates();
            void horizontalMovement(bool left);

    };

    class SquareSet: public GameObject {
        private:
            bool empty;
            Square*** squaresArray;
            int numberOfSquares;
                    
        public:
            static bool instanceActive;
            SquareSet(int startX, int startY);
            ~SquareSet();
            bool isEmpty();
            void fill(int w, int h);
            // Square* getSquare();
            Square*** getSquares();
            void moveDown();
            void horizontalMovement(bool left);
            bool nextToBorder(int w, bool left);
            void rotate();

    };

    class Game {
        private:
            bool gameOver;
            int width, height, squareWidth, squareHeight;
            SquareSet* squareSet;
            list<Square*> squaresList;
            void clearRow(int row, int perRow);
            void clearRows();

        public:
            Game(int w, int h);
            void init();
            void update();
            void addSquareSet();
            void deleteFigure();
            bool isGameOver() const;
            void setGameStatus(bool over);
            void moveSquareSet(bool left);
            void rotateSquareSet();
            // emscripten::val getSquareAtPostion(int position);
    };

}
