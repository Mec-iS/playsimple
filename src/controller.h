#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <vector>
#include <functional>
#include <iostream>

#include <SDL2/SDL.h>

#include "sprite.h"

class Controller;
using DirectionsMap = std::map<int, void(*)(Controller&& ctrl)>;

// moves
void moveLeft(Controller&& ctrl);
void moveRight(Controller&& ctrl);
void moveUp(Controller&& ctrl);
void moveDown(Controller&& ctrl);
void dash(Controller&& ctrl);
void checkBorders(Controller&& ctrl);

class Controller {
public:
  Controller() {
    keyboard_state_array = SDL_GetKeyboardState(NULL);

    skoob = Sprite();
    
    // build directions map in controller
    this->directions_mapping[SDL_SCANCODE_LEFT] = &moveLeft;
    this->directions_mapping[SDL_SCANCODE_UP] = &moveUp;
    this->directions_mapping[SDL_SCANCODE_RIGHT] = &moveRight;
    this->directions_mapping[SDL_SCANCODE_DOWN] = &moveDown;
    this->directions_mapping[SDL_SCANCODE_SPACE] = &dash;
  };
  ~Controller() {
    keyboard_state_array = NULL;
  };

  // keyboard state
  const Uint8* keyboard_state_array;

  // handle inputs
  void handleInput();

  DirectionsMap directions_mapping;
    // direction
  void resetDirection() {
          std::get<0>(direction) = 0;
          std::get<1>(direction) = 0;
  }
  std::tuple<int, int> direction;
  std::tuple<int, int> destination() {
    return std::make_tuple(
        (skoob.x + (std::get<0>(direction) * SPEED * 2)),
        (skoob.y + (std::get<1>(direction) * SPEED * 2)));
  }; // compute the destination of a dash
  void resetPosition(int reset_x, int reset_y);
  Sprite skoob;
};

#endif