#include "game.h"
#include "layout.h"
#include "controller.h"


Game::Game(): skip(false), isStarted(false), window(NULL), renderer(NULL) {
    std::cout << "Creating Game instance" << std::endl;
    this->controller = Controller();

    int i = 0;
    while (i < 4) {
        std::cout << "Creating boulders" << std::endl;
        SDL_Rect sprite;

        int rand = controller.generateRandomInt();

        sprite.w = rand * 10;
        sprite.h = rand * 10;

        std::tuple<int, int> pos = controller.generateRandomPosition();
        sprite.x = std::get<0>(pos);
        sprite.y = std::get<1>(pos);

        controller.boulders.emplace_back(std::make_tuple(sprite, true));
        i += 1;
    }

    // std::cout << i << std::endl;
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
    this->isStarted = true;
    run();
}

void Game::draw() {
    SDL_Rect sprite;

    // Clear
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    displayBoulders();    
    displaySkoob(std::move(sprite));

    SDL_RenderPresent(renderer);
}

void Game::displaySkoob(SDL_Rect&& sp) {

    sp.w = 20 ;
    sp.h = 20 ;
  
    // Get position
    std::tuple<int, int> pos = controller.skoob.getPosition();
    sp.x = std::get<0>(pos);
    sp.y = std::get<1>(pos);

    std::cout << "Skoob: " << sp.x << ", " << sp.y << std::endl;

    paintSkoob(&sp);
    checkBorders(std::move(controller), &sp);
}

void Game::displayBoulders() {
    std::cout << "Displaying boulders" << std::endl;
    // std::cout << "color " << std::get<0>(color) << std::get<1>(color) << std::get<2>(color) << std::endl;
    // std::cout << "position " << std::get<0>(pos) << ", " << std::get<1>(pos) << std::endl;

    for (std::tuple<SDL_Rect, bool> it : controller.boulders) {
        std::cout << "Boulder exists >>>>>> " << std::get<1>(it) << std::endl;
        if(std::get<1>(it)) {
           SDL_Rect* sprite = &std::get<0>(it);
           std::cout << "Sprite: " << sprite->x << ", " << sprite->y << std::endl;
       
           fillRect(sprite, 0, 0, 0);
        }
    }

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

void Game::paintSkoob(SDL_Rect* sp) {
    if (controller.isDashing) {
      fillRect(sp, 255, 0, 0);
      std::cout << "dashing >>>>" << controller.isDashing << std::endl;
    } else {
      fillRect(sp, 155, 155, 0);
    }
}

void Game::fillRect(SDL_Rect* rc, int r, int g, int b ) {
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, rc);
}

void Game::fpsChanged( int fps ) {
    std::string t =  "Skoob :: PlaySimple";
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

    while (this->isStarted) {
        frame_start = SDL_GetTicks();

        // handle events
        int quit = controller.handleInput();
        if (quit || this->controller.score > 3) { onQuit(); break; }
        
        // render sprites
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


