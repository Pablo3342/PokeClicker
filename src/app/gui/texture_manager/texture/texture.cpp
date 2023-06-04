#include "texture.hpp"

Texture::Texture(const char* t_path)
    : sdlTexture(nullptr),
      path(t_path),
      width(0),
      length(0)
{}

Texture::~Texture()
{
    deallocate();
}

///SETTING

bool Texture::loadTexture(SDL_Renderer* t_renderer) {
    SDL_Surface *loadedSurface = nullptr;
    deallocate();

    loadedSurface = IMG_Load(path);

    if (loadedSurface == nullptr) {
        printf("Image %s couldn't load, Error:%s\n", path, IMG_GetError());
        return false;
    } else {
        Uint32 colorkey = SDL_MapRGB(loadedSurface->format, 255, 255, 255);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, colorkey);

        sdlTexture = SDL_CreateTextureFromSurface(t_renderer, loadedSurface);

        SDL_FreeSurface(loadedSurface);
        loadedSurface = nullptr;

        if (sdlTexture == nullptr) {
            printf("Failed to convert surface into texture, Error:%s\n", SDL_GetError());
            return false;
        } else {
            deriveFileSize();
        }
    }
    return true;
}

///GETTING

int Texture::getWidth() {
    return width;
}

int Texture::getLength() {
    return length;
}

///SETTING

void Texture::deriveFileSize() {
    if (sdlTexture != nullptr) {
        SDL_QueryTexture(sdlTexture, nullptr, nullptr, &width, &length);
    } else {
        printf("Couldn't set texture size - texture is empty");
        width = 0;
        length = 0;
    }
}

///DEALLOCATE

void Texture::deallocate() {
    if (sdlTexture != nullptr) {
        SDL_DestroyTexture(sdlTexture);
        sdlTexture = nullptr;
    }
}