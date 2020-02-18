#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <vector>
#include <functional>

#include <SDL2/SDL.h>

#include "sprite.h"

using DirectionsMap = std::map<int, std::function<void()>>;

class Controller {
public:
  Controller() {
    keyboard_state_array = SDL_GetKeyboardState(NULL);
    
    // build directions map
    Sprite sp;
    directions_mapping = {
      { SDL_SCANCODE_LEFT, [&sp](){sp.moveLeft();} },
      { SDL_SCANCODE_UP, [&sp](){sp.moveUp();} },
      { SDL_SCANCODE_RIGHT, [&sp](){sp.moveRight();} },
      { SDL_SCANCODE_DOWN, [&sp](){sp.moveDown();} }
    };
  };
  ~Controller() {
    keyboard_state_array = NULL;
  };

  // keyboard state
  const Uint8* keyboard_state_array;

  // handle inputs
  void handleInput(Sprite &skoob);
private:
  DirectionsMap directions_mapping;
};

#endif