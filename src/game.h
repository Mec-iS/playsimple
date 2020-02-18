#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <map>
#include <SDL2/SDL.h>

#include "sprite.h"
#include "controller.h"

class Game {
public:
    Game();
    ~Game();
    // basic SDL setup-teardown methods
    void start();
    void stop() ;
    void run();
    void onQuit();
    
    // utility methods
    void fillRect(SDL_Rect* rc, int r, int g, int b );
    void fpsChanged(int fps);
    
    // simple event handlers, just encapsulate sprite methods
    void onMove(SDL_Event* event);
    void onDash(SDL_Event* event);

    // main loop methods
    void draw();
    void update();

private:
    bool skip;
    bool isStarted;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Controller controller;
};

#endif