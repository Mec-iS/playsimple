#include "controller.h"

void Controller::handleInput(Sprite &skoob) {
    if (keyboard_state_array[SDL_SCANCODE_LEFT] && keyboard_state_array[SDL_SCANCODE_UP]) {
        skoob.moveLeft();
        skoob.moveUp();
    } else if (keyboard_state_array[SDL_SCANCODE_RIGHT] && keyboard_state_array[SDL_SCANCODE_UP]) {
        skoob.moveRight();
        skoob.moveUp();
    } else if (keyboard_state_array[SDL_SCANCODE_LEFT] && keyboard_state_array[SDL_SCANCODE_DOWN]) {
        skoob.moveLeft();
        skoob.moveDown();
    } else if (keyboard_state_array[SDL_SCANCODE_RIGHT] && keyboard_state_array[SDL_SCANCODE_DOWN]) {
        skoob.moveRight();
        skoob.moveDown();
    }    

    if (keyboard_state_array[SDL_SCANCODE_LEFT]) {
        skoob.moveLeft();
    } else if (keyboard_state_array[SDL_SCANCODE_RIGHT]) {
        skoob.moveRight();
    } else if (keyboard_state_array[SDL_SCANCODE_UP]) {
        skoob.moveUp();
    } else if (keyboard_state_array[SDL_SCANCODE_DOWN]) {
        skoob.moveDown();
    }
    
    if (keyboard_state_array[SDL_SCANCODE_SPACE]) {
        skoob.isDashing = true;
        skoob.dash();
    } else {
        skoob.isDashing = false;
    }
}