#include <cstddef>
#include <cstdlib>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <math.h>

// source ~/emsdk/emsdk_env.sh

#include "objects.h"

using namespace G;            

EMSCRIPTEN_BINDINGS() {
    
    emscripten::class_<Game>("Game")     
      .constructor<int, int>() 
      .function("init", &Game::init)                                   
      .function("addSquareSet", &Game::addSquareSet)
      .function("deleteFigure", &Game::deleteFigure)
      .function("moveSquareSet", &Game::moveSquareSet)
      .function("rotateSquareSet", &Game::rotateSquareSet)
      // .function("getSquareAtPostion", &Game::getSquareAtPostion)
      .function("update", &Game::update);  

    emscripten::class_<GameObject>("GameObject")  
      .constructor<int, int>()
      // .property("y", &GameObject::getY, &GameObject::setY)
      .function("isStill", &GameObject::isStill)
      .function("setStill", &GameObject::setStill)
      .function("bottomRightCoordinates", &GameObject::bottomRightCoordinates, emscripten::allow_raw_pointers())
      .function("moveDown", &GameObject::moveDown);
      // .allow_subclass<Square>("Square");                        

    // emscripten::class_<Square>("Square")
    emscripten::class_<Square, emscripten::base<GameObject>>("Square")
      .constructor<int, int, int, int>() 
      .function("getX", &Square::getX)
      .function("getY", &Square::getY)
      .function("getYY", &Square::getYY) 
      .function("setX", &Square::setX) 
      .function("setY", &Square::setY)
      .function("getWidth", &Square::getWidth)  
      .function("getHeight", &Square::getHeight) 
      .function("bottomRightCoordinates", &Square::bottomRightCoordinates, emscripten::allow_raw_pointers());

    // emscripten::class_<Figure, emscripten::base<GameObject>>("Figure")
    //   .constructor<int, int, int, int>()
    //   .function("getNumberOfSquares", &Figure::getNumberOfSquares)
    //   .function("getSquares", &Figure::getSquares, emscripten::allow_raw_pointers())
    //   .function("bottomRightCoordinates", &Figure::bottomRightCoordinates, emscripten::allow_raw_pointers());

}