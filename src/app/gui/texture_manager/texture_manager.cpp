#include "texture_manager.hpp"

TextureManager::TextureManager()
    :   dialogBox("assets/dialogBox.png"),
        tileSheet("assets/tile_sheet.png"),
        player("assets/player.png"),
        playerFocus({0,0}),
        tilefocus({0,0}),
        font()
{}

TextureManager::~TextureManager()
{}

SDL_Texture* TextureManager::getTextTexture(SDL_Renderer* renderer, std::string text) {
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;

    const char* utf8Text = text.c_str();
    surface = TTF_RenderText_Blended_Wrapped(font, utf8Text, {89, 54, 37, 0}, config::NEW_LINE);

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    return texture;
}

void TextureManager::setTileFileFocus(int id) {
    Point<int> pos = { 0, 0 };
    pos.x = id % config::TILESHEET_W;
    pos.y = id / config::TILESHEET_W;

    tilefocus = pos;
}

void TextureManager::setPlayerFileFocus(int id) {
    Point<int> pos = { 0, 0 };
    pos.x = id % config::PLAYERSHEET_W;
    pos.y = id / config::PLAYERSHEET_W;

    playerFocus = pos;
}

void TextureManager::loadAllTextures(SDL_Renderer* sdlRenderer) {
    tileSheet.loadTexture(sdlRenderer);
    dialogBox.loadTexture(sdlRenderer);
    player.loadTexture(sdlRenderer);

    font = TTF_OpenFont("assets/font.ttf", config::FONT_SIZE);
}