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

    };

    class SquareSet: public GameObject {
        private:
            bool empty;
            Square*** sa;
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

    };

    class Figure: public GameObject {
        private:
            Square ***squaresArray;
            // vector<vector<FigureCell>> squaresVector;
            int numberOfSquares, arrayDim;
            void addSquare(int startX, int startY, int w, int h, int row, int column);
        
        public:
            Figure(int startX, int startY, int w, int h);
            ~Figure();
            // vector<vector<FigureCell>>* getSquares();
            Square*** getSquares();
            int getNumberOfSquares() const;
            int* bottomRightCoordinates();

    };

    class Game {
        private:
            bool gameOver;
            int width, height;
            SquareSet* squareSet;
            list<Square*> squaresList;

        public:
            Game(int w, int h);
            void init();
            void update();
            void addSquareSet();
            void deleteFigure();
            // emscripten::val getSquareAtPostion(int position);
    };

}
