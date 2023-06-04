#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <windows.h>
#include <stdio.h>

namespace sdlinit {
    ///LIBRARIES
    bool initLibraries();
    void closeLibraries();

    ///CONSOLE OUTPUT
    void openConsole();
    void closeConsole();
}