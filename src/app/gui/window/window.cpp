#include "window.hpp"

Window::Window()
    : instance(nullptr),
      WIDTH(config::WINDOW_WIDTH),
      LENGTH(config::WINDOW_HEIGHT)
{}

Window::~Window() {
    if (instance != nullptr) {
        SDL_DestroyWindow(instance);
        instance = nullptr;
    }
}

void Window::create(const char* title) {
    if (WIDTH > 0 && LENGTH > 0) {
        if (instance == nullptr) {
            instance = SDL_CreateWindow(
                title, 
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WIDTH, LENGTH,
                SDL_WINDOW_SHOWN);

            if (instance == nullptr) {
                printf("Failed to create window, Error: %s\n", SDL_GetError());
            }
        } else {
            printf("Failed to create window, Error: window is already existing\n");
        }
    } else {
        printf("Failed to create window, Error: width or high incorrect\n");
    }
}