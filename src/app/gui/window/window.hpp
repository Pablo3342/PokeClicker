#pragma once

#include "../../../config/config.hpp"

#include <SDL2/SDL.h>
#include <string>

class Window {
public:
    Window();
    ~Window();

    void create(const char* title);

    SDL_Window* instance;

private:
    const int WIDTH;
    const int LENGTH;
};