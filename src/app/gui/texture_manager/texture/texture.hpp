#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

class Texture {
public:
    Texture(const char* t_path);
    ~Texture();

    ///VARIABLES
    SDL_Texture* sdlTexture;

    ///SETTING
    bool loadTexture(SDL_Renderer* t_renderer);
    void deriveFileSize();

    ///GETTING
    int getWidth();
    int getLength();

private:
    ///VARIABLES
    const char* path;
    int length;
    int width;

    ///DEALLOCATE
    void deallocate();
};