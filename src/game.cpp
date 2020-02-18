#include "game.h"
#include "layout.h"
#include "controller.h"

Game::Game(): skip(false), isStarted(false), window(NULL), renderer(NULL) {
    controller = Controller();
}

Game::~Game() {
    this->stop();
}

void Game::start() {
    int flags = SDL_WINDOW_SHOWN;
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        return;
    }
    if (SDL_CreateWindowAndRenderer(WIN_WIDTH, WIN_HEIGHT, flags, &window, &renderer)) {
        return;
    }
    this->isStarted = 1;
    run();
}

void Game::draw() {
    SDL_Rect sprite;
    
    // Clear
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    // Render
    std::tuple pos = skoob.getPosition();
    sprite.x = std::get<0>(pos);
    sprite.y = std::get<1>(pos);

    this->checkBorders();

    sprite.w = 20 ;
    sprite.h = 20 ;

    std::cout << sprite.x << ", " << sprite.y << std::endl;

    if (skoob.isDashing) {
      fillRect(&sprite, 255, 0, 0);
    } else {
      fillRect(&sprite, 155, 0, 0);
    }
    
    SDL_RenderPresent(renderer);
}

void Game::stop() {
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_Quit();
}

void Game::fillRect(SDL_Rect* rc, int r, int g, int b ) {
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, rc);
}

void Game::fpsChanged( int fps ) {
    std::string t =  "PlaySimple";
    std::cout << t << " FPS " << fps << std::endl;
    SDL_SetWindowTitle(window, t.c_str());
}

void Game::onQuit() {
    this->isStarted = false;
}

void Game::run() {
    int init_timestamp = SDL_GetTicks();
    int frame_start;
    int frame_end;
    int frame_duration;
    int frame_count = 0;
    this->isStarted = true;

    SDL_Event event;
    while (this->isStarted) {
        frame_start = SDL_GetTicks();

        // check system keys
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                    case SDL_QUIT:    onQuit(); break;
            }
        }
        this->update();
        this->draw();

        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle
        // takes.
        frame_count++;
        frame_duration = frame_end - frame_start;

        // After every second, update the window title.
        if (frame_end - init_timestamp >= 1000) {
            frame_count = 0;
            init_timestamp = frame_end;
        }

        // If the time for this frame is too small (i.e. frame_duration is
        // smaller than the target ms_per_frame), delay the loop to
        // achieve the correct frame rate.
        if (frame_duration < INTERVAL) {
            SDL_Delay(INTERVAL - frame_duration);
        }
    }
}

void Game::update() {
    controller.handleInput(skoob);
    skoob.resetDirection();
}

