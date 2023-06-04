#include "gui.hpp"

GUI::GUI(AppManager* t_appManager, World* t_world)
    :   textureManager(),
        camera(t_appManager),
        world(t_world),
        renderer(t_appManager, &textureManager, &t_world->fightOverlay, &camera, t_world),
        window()
{}

GUI::~GUI()
{}

void GUI::process() {
    camera.setFocus(world->player.pos);
    camera.process();
    renderer.render();
}

void GUI::create() {
    window.create(config::APP_TITLE);
    renderer.create(window.instance);
    textureManager.loadAllTextures(renderer.instance);
}