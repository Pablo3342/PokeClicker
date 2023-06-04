#pragma once

#include <SDL2/SDL.h>
#include <utility>

class RectBuilder {
public:
    RectBuilder();
    ~RectBuilder();

    SDL_Rect&& build();
    RectBuilder& x(int x);
    RectBuilder& y(int y);
    RectBuilder& w(int w);
    RectBuilder& h(int h);

private:
    SDL_Rect rect;
};