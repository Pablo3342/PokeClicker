#include "app.hpp"

App::App() 
    :   appManager(),
        world(&appManager),
        gui(&appManager, &world)
{
    gui.create();
}

App::~App() 
{}

void App::run() {
    while (appManager.info.gameLoop) {
        appManager.process();
        world.process();
        gui.process();
    }
}

void App::init() {
    appManager.startTimer();
}