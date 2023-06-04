#pragma once

#include "../../config/config.hpp"
#include "texture_manager/texture_manager.hpp"
#include "renderer/renderer.hpp"
#include "camera/camera.hpp"
#include "../world/world.hpp"
#include "window/window.hpp"

class AppManager;

class GUI {
public:
    GUI(AppManager* t_appManager, World* t_world);
    ~GUI();

    void process();

    void create();

private:
    TextureManager textureManager;
    World* world;
    Camera camera;
    Renderer renderer;
    Window window;
};