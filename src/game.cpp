#include "game.h"
#include "layout.h"
#include "controller.h"


Game::Game(): skip(false), isStarted(false), window(NULL), renderer(NULL) {
    this->controller = Controller();

    for (int i = 0; i == 4; i++) {
        SDL_Rect sprite;

        int rand = controller.generateRandomInt();
        SDL_Color color = controller.generateRandomColor();

        sprite.w = rand * 10;
        sprite.h = rand * 10;

        std::tuple<int, int> pos = controller.generateRandomPosition();
        sprite.x = std::get<0>(pos);
        sprite.y = std::get<1>(pos);

        controller.boulders.push_back(std::make_tuple(sprite, color));
    }
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
    
    displaySkoob(std::move(sprite));
    displayBoulders();

    SDL_RenderPresent(renderer);
}

void Game::displaySkoob(SDL_Rect&& sp) {

    sp.w = 20 ;
    sp.h = 20 ;
  
    // Get position
    std::tuple<int, int> pos = controller.skoob.getPosition();
    sp.x = std::get<0>(pos);
    sp.y = std::get<1>(pos);

    checkBorders(std::move(controller));

    paintSkoob(&sp);
}

void Game::displayBoulders() {
    // std::cout << "boulder created " << std::endl;
    // std::cout << "color " << std::get<0>(color) << std::get<1>(color) << std::get<2>(color) << std::endl;
    // std::cout << "position " << std::get<0>(pos) << ", " << std::get<1>(pos) << std::endl;

    for (std::tuple<SDL_Rect, SDL_Color> it : controller.boulders) {
       SDL_Rect* sprite = &std::get<0>(it);
       SDL_Color color = std::get<1>(it);
       
       fillRect(sprite, color.r, color.g, color.b);
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
        if (quit) { onQuit(); break; }
        
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


