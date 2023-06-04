#include "sdlinit.hpp"

///LIBRARIES

bool sdlinit::initLibraries() {
    // INITIALIZE SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize SDL library, Error:\n%s\n", SDL_GetError());
        return false;
    }

    // INITIALIZE SDL2_image
    if (!(IMG_Init(IMG_INIT_PNG) && IMG_INIT_PNG)) {
        printf("Failed to initialize IMG library, Error:\n%s\n", IMG_GetError());
        return false;
    }

    // INITIALIZE SDL2_ttf
    if (TTF_Init() < 0) {
        printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void sdlinit::closeLibraries() {
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

///CONSOLE OUTPUT

void sdlinit::openConsole() {
    AllocConsole();
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
}

void sdlinit::closeConsole() {
    fclose(stdout);
    fclose(stderr);
}