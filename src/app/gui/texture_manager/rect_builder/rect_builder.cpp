#include "rect_builder.hpp"

///CONSTRUCTOR & DESTRUCTOR

RectBuilder::RectBuilder()
{}

RectBuilder::~RectBuilder()
{}

///OPERATIONS

SDL_Rect&& RectBuilder::build(){
        return std::move(rect);
}

RectBuilder& RectBuilder::x(int x){
    rect.x = x;
    return *this;
}

RectBuilder& RectBuilder::y(int y){
    rect.y = y;
    return *this;
}

RectBuilder& RectBuilder::w(int w){
    rect.w = w;
    return *this;
}

RectBuilder& RectBuilder::h(int h){
    rect.h = h;
    return *this;
}