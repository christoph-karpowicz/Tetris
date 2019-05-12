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
      .function("getGameToString", &Game::getGameToString)
      .function("reset", &Game::reset)
      .function("getPaused", &Game::getPaused)
      .function("setPaused", &Game::setPaused)
      .function("getStarted", &Game::getStarted)
      .function("setStarted", &Game::setStarted)
      .function("update", &Game::update);

    emscripten::class_<Square, emscripten::base<GameObject>>("Square")
      .constructor<int, int, int, int>() 
      .function("getWidth", &Square::getWidth)  
      .function("getHeight", &Square::getHeight) 
      .function("bottomRightCoordinates", &Square::bottomRightCoordinates, emscripten::allow_raw_pointers());

    emscripten::function("verticallySymetric", &G::verticallySymetric, emscripten::allow_raw_pointers());

}