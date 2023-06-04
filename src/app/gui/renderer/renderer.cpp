#include "renderer.hpp"

Renderer::Renderer(AppManager* t_appManager, TextureManager* t_textureManager, FightOverlay* t_fightOverlay, Camera* t_camera, World* t_world)
    :   instance(nullptr),
        textureManager(t_textureManager),
        appManager(t_appManager),
        camera(t_camera),
        world(t_world),
        proffesorOverlay(t_appManager),
        fightOverlay(t_fightOverlay)
{
    init();
}

Renderer::~Renderer() {
    if (instance != nullptr) {
        SDL_DestroyRenderer(instance);
        instance = nullptr;
    }
}

void Renderer::render() {
    if (appManager->info.frames2Render > 0) {
        ++appManager->info.allFramesRendered;
        renderFrame();
    }
}

void Renderer::renderFrame() {
    SDL_RenderClear(instance);
    renderMap();
    renderDialogBox(appManager->info.dialogBox);
    SDL_RenderPresent(instance);
}

void Renderer::renderDialogBox(DIALOG_BOX box) {
    if (box == NONE) {
        return;
    }

    SDL_Rect fileRect;
    fileRect.x = 0;
    fileRect.y = 0;
    fileRect.w = textureManager->dialogBox.getWidth();
    fileRect.h = textureManager->dialogBox.getLength();

    SDL_Rect scaledRect;
    scaledRect.x = config::DIALOG_BOX_X;
    scaledRect.y = config::DIALOG_BOX_Y;
    scaledRect.w = config::DIALOG_BOX_W;
    scaledRect.h = config::DIALOG_BOX_H;

    SDL_SetRenderDrawColor(instance, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderCopy(instance, textureManager->dialogBox.sdlTexture, &fileRect, &scaledRect);

    switch (appManager->info.dialogBox) {
        case PROFFESOR:
            if (appManager->info.dialogBoxID >= proffesorOverlay.text.size()) {
                appManager->info.dialogBox = NONE;
                appManager->info.lockPlayerMovement = false;
                appManager->info.dialogBoxID = 0;
                return;
            }
            renderText(proffesorOverlay.text[appManager->info.dialogBoxID]);
        break;

        case FIGHT:
            if (appManager->info.dialogBoxID >= fightOverlay->text.size()) {
                --appManager->info.dialogBoxID;
            }
            renderText(fightOverlay->text[appManager->info.dialogBoxID]);
        break;

        case NONE:
            appManager->info.dialogBoxID = 0;
       break;
    }
}

void Renderer::renderText(std::string text) {
    SDL_Texture* texture = textureManager->getTextTexture(instance, text);

    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &srcRect.w, &srcRect.h);

    Point<float> dialogBoxCenter;
    dialogBoxCenter.x = DIALOG_BOX_X + DIALOG_BOX_W / 2;
    dialogBoxCenter.y = DIALOG_BOX_Y + DIALOG_BOX_H / 2;

    SDL_Rect dstRect;
    dstRect.x = dialogBoxCenter.x - srcRect.w / 2;
    dstRect.y = dialogBoxCenter.y - srcRect.h / 2;
    dstRect.w = srcRect.w;
    dstRect.h = srcRect.h;

    SDL_RenderCopy(instance, texture, &srcRect, &dstRect);

    SDL_DestroyTexture(texture);
}

void Renderer::renderMap() {
    
    for (int i = camera->hitbox.y; i < camera->hitbox.h; ++i) {
        for (int j = camera->hitbox.x; j < camera->hitbox.w; ++j) {
            renderTile(j, i);
        }
    }

    renderPlayer();
}

void Renderer::renderPlayer() {
    SDL_Rect scaledTile = getScaledTileRect();
    Point<float> pos = world->player.pos;
    Size<float> size = {world->player.box.w, world->player.box.h};
    Point<float> topLeft = { 0, 0 };
    
    camera->adjustPos(pos);

    //topLeft.x = (pos.x - world->player.box.w * 0.5);
    //topLeft.y = (pos.y - world->player.box.h * 0.5);
    //camera->smoothPos(topLeft);

    //pos.x = rint((pos.x) * scaledTile.w);
    //pos.y = rint((pos.y) * scaledTile.h);
    //camera->smoothPos(pos);

    SDL_Rect originalTile = { 0, 0, config::PLAYERSIZE_W, config::PLAYERSIZE_H };

    SDL_Rect scrRect;
    scrRect.x = rint((pos.x - size.w * 0.5) * scaledTile.w);
    scrRect.y = rint((pos.y - size.h * 0.5) * scaledTile.h);
    scrRect.w = size.w * scaledTile.w;
    scrRect.h = size.h * scaledTile.h;

    Point<int> sheetPlayerPos = { 0, 0 };
    textureManager->setPlayerFileFocus(appManager->getCurrentAnimation());
    sheetPlayerPos = textureManager->playerFocus;
    originalTile.x = sheetPlayerPos.x * config::PLAYERSIZE_W;
    originalTile.y = sheetPlayerPos.y * config::PLAYERSIZE_H;

    SDL_SetRenderDrawColor(instance, 0xFF, 0xFF, 0xFF,0);
    SDL_RenderCopy(instance, textureManager->player.sdlTexture, &originalTile, &scrRect);
    //SDL_RenderFillCircle(instance, pos.x, pos.y, world->player.box.w * 0.5 * scaledTile.w);
    SDL_SetRenderDrawColor(instance, 0xFF, 0xFF, 0xFF, 0);
}

void Renderer::init() {

}

void Renderer::renderTile(const int& x, const int& y) {
    ///VARIABLES
    SDL_Texture* sheet = textureManager->tileSheet.sdlTexture;

    //initialize positions
    Point<int> sheetTilePos = { 0, 0};
    Point<float> tilePos = { x, y };

    //render forms
    SDL_Rect originalTile = { 0, 0, config::TILE_SIZE, config::TILE_SIZE };
    SDL_Rect scaledTile = getScaledTileRect();

    ///ADJUST POSITION TO CAMERA
    camera->adjustPos(tilePos);

    // MAP CHANGE
    int id = 0;
    switch (appManager->info.map) {
        case SPAWN:
            id = world->spawn.map[y][x];
        break;

        case FOREST:
            id = world->forest.map[y][x];
        break;

        case BEACH:
            id = world->beach.map[y][x];
        break;
    }

    ///SET TEXTURE POSITION in textureManager
    textureManager->setTileFileFocus(id);
    sheetTilePos = textureManager->tilefocus;

    ///SET TILE TO RENDER FROM
    originalTile.x = originalTile.w * sheetTilePos.x;
    originalTile.y = originalTile.h * sheetTilePos.y;

    ///MAKE PLACE TO RENDER TILE
    scaledTile.x = rint(tilePos.x * scaledTile.w);
    scaledTile.y = rint(tilePos.y * scaledTile.h);

    ///RENDERING FUNCTION
    //render to screen scaledTile from originalTile
    //texture using sheet from textureManager
    SDL_RenderCopy(instance, sheet, &originalTile, &scaledTile);
}

SDL_Rect Renderer::getScaledTileRect() {
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = rint(TILE_SIZE * appManager->info.worldScale);
    rect.h = rint(TILE_SIZE * appManager->info.worldScale);

    return rect;
}

// SETTING

void Renderer::create(SDL_Window* sdlWindow) {
    if (instance == nullptr) {
        instance = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);

        if (instance == nullptr) {
            printf("Failed to create renderer, Error: %s\n", SDL_GetError());
            throw std::runtime_error(SDL_GetError());
        }
        else {
            SDL_SetRenderDrawColor(instance, 0xFF, 0xFF, 0xFF, 0xFF);
        }
    }
    else {
        printf("Failed to assign renderer to window, Error: renderer is already assigned\n");
    }
}

int SDL_RenderDrawCircle(SDL_Renderer* renderer, float x, float y, float radius) {
    float offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPointF(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPointF(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPointF(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPointF(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPointF(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPointF(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPointF(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPointF(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int SDL_RenderFillCircle(SDL_Renderer* renderer, float x, float y, float radius) {
    float offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLineF(renderer, x - offsety, y + offsetx,
            x + offsety, y + offsetx);
        status += SDL_RenderDrawLineF(renderer, x - offsetx, y + offsety,
            x + offsetx, y + offsety);
        status += SDL_RenderDrawLineF(renderer, x - offsetx, y - offsety,
            x + offsetx, y - offsety);
        status += SDL_RenderDrawLineF(renderer, x - offsety, y - offsetx,
            x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}