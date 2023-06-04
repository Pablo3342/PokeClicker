#pragma once

#include "app_manager/app_manager.hpp"
#include "world/world.hpp"
#include "gui/gui.hpp"

class App {
public:
    App();
    ~App();

    void init();
    void run();

private:
    AppManager appManager;
    World world;
    GUI gui;
};