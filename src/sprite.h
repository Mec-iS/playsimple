#ifndef SPRITE_H
#define SPRITE_H

#include <tuple>
#include <map>

#include "layout.h"

class Sprite {
public:
    Sprite() :x(0), y(STARTING_POSITION) {}
    
    // moves
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void resetPosition(int reset_x, int reset_y) {
        x = reset_x;
        y = reset_y;
        prev_x = 0;
        prev_y = STARTING_POSITION;
    };
    std::tuple<int, int> getPosition() {
        return std::make_tuple(x, y);
    };

    // moving
    void resetDirection() {
            std::get<0>(direction) = 0;
            std::get<1>(direction) = 0;
    }
    std::tuple<int, int> direction;
    bool isMoving() {
        return x == prev_x && y == prev_y;
    };

    int charge; // charge the lenght of the dash
    bool isDashing;  // jumping state
    int speed;
    std::tuple<int, int> destination() {
       return std::make_tuple(
           (x + (std::get<0>(direction) * SPEED * 2)),
           (y + (std::get<1>(direction) * SPEED * 2)));
    }; // compute the destination of a dash

    void dash(){
        prev_x = x;
        prev_y = y;
        std::tuple<int, int> dash_to = destination();
        x = std::get<0>(dash_to);
        y = std::get<1>(dash_to);
    };

private:
    // space
    int x, y;
    int prev_x, prev_y;
};

#endif