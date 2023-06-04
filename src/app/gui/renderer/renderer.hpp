#pragma once

#include "../../app_manager/app_manager.hpp"
#include "../texture_manager/texture_manager.hpp"
#include "../proffesor_overlay/proffesor_overlay.hpp"
#include "../fight_overlay/fight_overlay.hpp"
#include "../camera/camera.hpp"
#include "../../world/world.hpp"

#include <iostream>
#include <math.h>

using namespace config;

class Renderer {
public:
    Renderer(AppManager* t_appManager, TextureManager* t_textureManager, FightOverlay* t_fightOverlay, Camera* t_camera, World* t_world);
    ~Renderer();

    SDL_Renderer* instance;

    void render();
    void renderFrame();

    void create(SDL_Window* window);

private:
    TextureManager* textureManager;
    AppManager* appManager;
    Camera* camera;
    World* world;

    ProffesorOverlay proffesorOverlay;
    FightOverlay* fightOverlay;

    void renderMap();
    void renderPlayer();
    void renderTile(const int& x, const int& y);
    void renderDialogBox(DIALOG_BOX box);
    void renderText(std::string text);
    void init();

    SDL_Rect getScaledTileRect();
};

int SDL_RenderDrawCircle(SDL_Renderer* renderer, float x, float y, float radius);
int SDL_RenderFillCircle(SDL_Renderer* renderer, float x, float y, float radius);