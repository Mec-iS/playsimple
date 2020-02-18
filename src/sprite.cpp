#include "sprite.h"
#include "layout.h"

void Sprite::moveLeft(){
    prev_x = x;
    x -= SPEED;
    std::get<0>(direction) = -1;
}

void Sprite::moveRight(){
    prev_x = x;
    x += SPEED;
    std::get<0>(direction) = 1;
}

void Sprite::moveUp(){
    prev_y = y;
    y -= SPEED;
    std::get<1>(direction) = -1;
}

void Sprite::moveDown(){
    prev_y = y;
    y += SPEED;
    std::get<1>(direction) = 1;
}