#pragma once

#include "texture/texture.hpp"
#include "../../../config/config.hpp"
#include "../../../struct/struct.hpp"

#include <SDL2/SDL_ttf.h>
#include <string>

class TextureManager {
public: 
    TextureManager();
    ~TextureManager();

    Texture dialogBox;
    Texture tileSheet;
    Texture player;

    Point<int> playerFocus;
    Point<int> tilefocus;
    TTF_Font* font;

    SDL_Texture* getTextTexture(SDL_Renderer* renderer, std::string text);

    // set file 'cursor' to specific tile in sheet
    void setTileFileFocus(int id);
    void setPlayerFileFocus(int id);

    void loadAllTextures(SDL_Renderer* sdlRenderer);
};