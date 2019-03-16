#include <emscripten.h>
#include <emscripten/bind.h>
#include <vector>
#include <string>

using namespace std;

namespace G {

    class Shape {
        protected:
            int width, height, x, y;
            bool still;

        public:
            Shape(int startX, int startY, int w, int h);
            void moveDown();
            int getX();
            emscripten::val getYY();
            int getY();
            void setX(int newX);
            void setY(int newY);
            int getWidth();
            int getHeight();
            bool isStill();

    };

    class Square: public Shape {
        public:
            Square(int startX, int startY, int w, int h);

    };

    class Game {
        private:
            int width, height;
            vector<Square> squaresVector;

        public:
            Game(int w, int h);
            void init();
            void update();
            void addSquare();
            void deleteSquare();
            emscripten::val getSquareAtPostion(int position);
    };

}
