#ifndef SPRITE_H
#define SPRITE_H

#include <tuple>
#include <map>

#include "layout.h"


class Sprite {
public:
    Sprite() :x(0), y(STARTING_POSITION) {}
    
    std::tuple<int, int> getPosition() {
        return std::make_tuple(x, y);
    };

    bool isMoving() {
        return x == prev_x && y == prev_y;
    };

    bool isDashing;  // jumping state

    // space
    int x, y;
    int prev_x, prev_y;
};

#endif