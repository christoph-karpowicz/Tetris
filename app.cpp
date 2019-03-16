#include <cstddef>
#include <cstdlib>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <math.h>

// source ~/emsdk/emsdk_env.sh

#include "objects.h"

using namespace G;

class Main {
    private: 
        int i, j;
    public:
        Main(int i) : i(i) {} 
        emscripten::val showi() {
            emscripten::val returnVal = emscripten::val::object();
            returnVal.set("i", emscripten::val(this->i));
            return returnVal;
        }
        void set_j(int num) { 
            this->j = num;
        }
        emscripten::val get_j() {
            emscripten::val returnVal = emscripten::val::object();
            returnVal.set("j", emscripten::val(this->j));
            return returnVal; 
        }
}; 

EMSCRIPTEN_BINDINGS(tetris) {
    
    emscripten::class_<Main>("Main")
      .constructor<int>()
      .function("showi", &Main::showi)
      .function("set_j", &Main::set_j)
      .function("get_j", &Main::get_j);
      
    emscripten::class_<Game>("Game")
      .constructor<int, int>()
      .function("init", &Game::init)
      .function("addSquare", &Game::addSquare)
      .function("deleteSquare", &Game::deleteSquare)
      .function("getSquareAtPostion", &Game::getSquareAtPostion)
      .function("update", &Game::update);

    emscripten::class_<Shape>("Shape")  
      .constructor<int, int, int, int>()
      .function("getX", &Shape::getX)
      .function("getY", &Shape::getY)
      .function("setX", &Shape::setX)
      .function("setY", &Shape::setY)
      .property("y", &Shape::getY, &Shape::setY)
      .function("getYY", &Shape::getYY)
      .function("getWidth", &Shape::getWidth)
      .function("getHeight", &Shape::getHeight)
      .function("isStill", &Shape::isStill)
      .function("moveDown", &Shape::moveDown)
      .allow_subclass<Square>("Square");

    emscripten::class_<Square>("Square")
      .constructor<int, int, int, int>();

}