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
      .function("moveSquareSet", &Game::moveSquareSet)
      .function("rotateSquareSet", &Game::rotateSquareSet)
      .function("getState", &Game::getState)
      .function("reset", &Game::reset)
      .function("setIsStopped", &Game::setIsStopped)
      .function("update", &Game::update);

    emscripten::class_<GameObject>("GameObject")
      .constructor<int, int>()
      .function("isStill", &GameObject::isStill)
      .function("setStill", &GameObject::setStill)
      .function("bottomRightCoordinates", &GameObject::bottomRightCoordinates, emscripten::allow_raw_pointers())
      .function("moveDown", &GameObject::moveDown);

    emscripten::class_<Square, emscripten::base<GameObject>>("Square")
      .constructor<int, int, int, int>() 
      .function("getWidth", &Square::getWidth)  
      .function("getHeight", &Square::getHeight) 
      .function("bottomRightCoordinates", &Square::bottomRightCoordinates, emscripten::allow_raw_pointers());

    emscripten::function("verticallySymetric", &G::verticallySymetric, emscripten::allow_raw_pointers());

}