#include <emscripten.h>
#include <emscripten/bind.h>
#include <vector>
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
            int* bottomRightCoordinates();
            
    };

    class Square: public GameObject {
        private:
            int width, height;
        
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

    class Figure {
        public:
            Figure(int startX, int startY, int w, int h);

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
