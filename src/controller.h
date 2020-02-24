#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <vector>
#include <functional>
#include <iostream>
#include <random>

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
void checkBorders(Controller&& ctrl, SDL_Rect*);


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
  int handleInput();
  
  // random
  int generateRandomInt() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist4(2,6);
    return dist4(rng);
  }

  // SDL_Color generateRandomColor() {
  //   std::random_device dev;
  //   std::mt19937 rng(dev());
  //   std::uniform_int_distribution<std::mt19937::result_type> dist255(50,255);
  //   // return SDL_Color { r: (Uint8) dist255(rng), g: (Uint8) dist255(rng),
  //   //                    b: (Uint8) dist255(rng), a: SDL_ALPHA_OPAQUE };
  //   SDL_Color color = SDL_Color { r: 0, g: 0,
  //                      b: 0, a: SDL_ALPHA_OPAQUE };
  //   return color;
  // }

  std::tuple<int, int> generateRandomPosition() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distWidth(20, WIN_WIDTH);
    std::uniform_int_distribution<std::mt19937::result_type> distHeight(20, WIN_HEIGHT);
    return std::make_tuple(distWidth(rng), distHeight(rng));
  }
  
  // dashing state of the sprite
  bool isDashing = false;  // dashing state

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
  std::vector<std::tuple<SDL_Rect, bool>> boulders;

  int score = 0;
};

#endif