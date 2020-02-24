#include "controller.h"

#include <iterator>
#include <tuple>
#include <algorithm>


// moves
void moveLeft(Controller&& ctrl) {
    ctrl.skoob.prev_x = ctrl.skoob.x;
    ctrl.skoob.x -= SPEED;
    int curr_y_dir = std::get<1>(ctrl.direction);
    ctrl.direction = std::make_tuple(-1, curr_y_dir);
};

void moveRight(Controller&& ctrl) {
    ctrl.skoob.prev_x = ctrl.skoob.x;
    ctrl.skoob.x += SPEED;
    int curr_y_dir = std::get<1>(ctrl.direction);
    ctrl.direction = std::make_tuple(1, curr_y_dir);
};

void moveUp(Controller&& ctrl) {
    ctrl.skoob.prev_y = ctrl.skoob.y;
    ctrl.skoob.y -= SPEED;
    int curr_x_dir = std::get<0>(ctrl.direction);
    ctrl.direction = std::make_tuple(curr_x_dir, -1);
};

void moveDown(Controller&& ctrl) {
    ctrl.skoob.prev_y = ctrl.skoob.y;
    ctrl.skoob.y += SPEED;
    int curr_x_dir = std::get<0>(ctrl.direction);
    ctrl.direction = std::make_tuple(curr_x_dir, 1);
};

void dash(Controller&& ctrl) {
    ctrl.skoob.prev_x = ctrl.skoob.x;
    ctrl.skoob.prev_y = ctrl.skoob.y;
    std::tuple<int, int> dash_to = ctrl.destination();
    std::cout << "DASH TO: " << std::get<0>(ctrl.direction) << ", " << std::get<1>(ctrl.direction) << std::endl;
    ctrl.skoob.x = std::get<0>(dash_to);
    ctrl.skoob.y = std::get<1>(dash_to);
    ctrl.resetDirection();
};

// void erase(std::vector<SDL_Rect>& v, SDL_Rect elem) {
//     std::vector<SDL_Rect>::iterator it = remove(v.begin(), v.end(), elem);
//     v.erase(it, v.end());
// }

void checkBorders(Controller&& ctrl, SDL_Rect* sp) {
    auto pos = ctrl.skoob.getPosition();
    
    // check external borders
    if ((0 > std::get<0>(pos) || 0 > std::get<1>(pos)) || 
        (std::get<0>(pos) > WIN_WIDTH || std::get<1>(pos) > WIN_HEIGHT)) {
        ctrl.resetPosition(0, STARTING_POSITION);
    }

    // check boulders collission
    int i = 0;
    int idx = -1;
    for (std::tuple<SDL_Rect, bool>& boulder: ctrl.boulders) {
        if(std::get<1>(boulder)) {
            SDL_Rect b = std::get<0>(boulder);
            if (SDL_HasIntersection(sp, &b)) {
                std::cout << "Collision" << std::endl;
                idx = i;
                std::cout << "Boulder destroyed " << idx << std::endl;
                std::get<1>(boulder) = false;
                std::cout << "Boulder exists " << std::get<1>(boulder) << std::endl;
            }
        }
        i++;
    }

}


// controller's methods
void Controller::resetPosition(int reset_x, int reset_y) {
    skoob.x = reset_x;
    skoob.y = reset_y;
    skoob.prev_x = 0;
    skoob.prev_y = STARTING_POSITION;
};

int Controller::handleInput() {
    SDL_Event event;
    
    // check system keys
    while (SDL_PollEvent(&event)) {
        // quit behaviour
        if (keyboard_state_array[SDL_SCANCODE_ESCAPE]) {
            return 1;
        }
        switch (event.type) {
            case SDL_QUIT: return 1;
            // case SDL_KEYUP: 
            //     switch (event.key.keysym.scancode) {
            //         case SDL_SCANCODE_SPACE:
            //         // end dash behaviour
            //         isDashing = false;
            //         std::cout << "KEYUP - dashing " << isDashing << std::endl;
            //     };
            // case SDL_KEYDOWN: 
            //     switch (event.key.keysym.scancode) {
            //         case SDL_SCANCODE_SPACE:
            //         // end dash behaviour
            //         isDashing = true;
            //         std::cout << "KEYDOWN - dashing " << isDashing << std::endl;
            //     }
        }
    }

    // loop through the ```keys->methods``` map
    std::map<int, void(*)(Controller&& ctrl)>::iterator it;
    for (it = directions_mapping.begin(); it != directions_mapping.end(); it++) {
        int kcode = std::get<0>((*it));

        // if key is pressed
        if (keyboard_state_array[kcode]) {
            // execute mapped method
            std::get<1>((*it))(std::move(*this));

            if (kcode == SDL_SCANCODE_SPACE) {
                isDashing = true;
                std::cout << "dashing " << isDashing << std::endl;
            }
        }
        isDashing = false;
    };

    return 0;

}