#include "controller.h"

#include <iterator>


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
    ctrl.skoob.isDashing = true;
    ctrl.skoob.prev_x = ctrl.skoob.x;
    ctrl.skoob.prev_y = ctrl.skoob.y;
    std::tuple<int, int> dash_to = ctrl.destination();
    std::cout << "DASH TO: " << std::get<0>(ctrl.direction) << ", " << std::get<1>(ctrl.direction) << std::endl;
    ctrl.skoob.x = std::get<0>(dash_to);
    ctrl.skoob.y = std::get<1>(dash_to);
    ctrl.resetDirection();
};

void checkBorders(Controller&& ctrl) {
    auto pos = ctrl.skoob.getPosition();
    
    if ((0 > std::get<0>(pos) || 0 > std::get<1>(pos)) || 
        (std::get<0>(pos) > WIN_WIDTH || std::get<1>(pos) > WIN_HEIGHT)) {
        ctrl.resetPosition(0, STARTING_POSITION);
    }
    ctrl.skoob.isDashing = false;
}


// controller's methods
void Controller::resetPosition(int reset_x, int reset_y) {
    skoob.isDashing = false;
    skoob.x = reset_x;
    skoob.y = reset_y;
    skoob.prev_x = 0;
    skoob.prev_y = STARTING_POSITION;
};

void Controller::handleInput() {
    // loop through the ```keys->methods``` map
    std::map<int, void(*)(Controller&& ctrl)>::iterator it;
    for (it = directions_mapping.begin(); it != directions_mapping.end(); it++) {
        int kcode = std::get<0>((*it));

        // if key is pressed
        if (keyboard_state_array[kcode]) {
            // execute mapped method
            std::get<1>((*it))(std::move(*this));
            std::cout << "dashing " << this->skoob.isDashing << std::endl;
        }
    };

}