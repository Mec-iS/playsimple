#ifndef SPRITE_H
#define SPRITE_H

#include <tuple>
#include <map>

#include "layout.h"


class Sprite {
public:
    Sprite() :x(0), y(STARTING_POSITION) {}
    
    std::tuple<int, int> const getPosition() {
        return std::make_tuple(x, y);
    };

    bool const isMoving() {
        return x == prev_x && y == prev_y;
    };

    // space
    int x, y;
    int prev_x, prev_y;
};

#endif