#include "camera.hpp"

Camera::Camera(AppManager* t_appManager)
    :   appManager(t_appManager),
        tileHitbox({ 0, 0, 0, 0 }),
        hitbox({ 0.0f, 0.0f, 0.0f, 0.0f }),
        screenCapacity({ 0.0f, 0.0f }),
        scaledTile({ 0.0f, 0.0f }),
        mapEnd({ 0.0f, 0.0f }),
        pos({0, 0})
{
    calcScaledTile();
    calcScreenCapacity();
    calcMapEnd();
}

Camera::~Camera()
{}

///PROCESS

void Camera::process() {
    //processZoom();
    calcHitbox();
    calcTileHitbox();
}

void Camera::processZoom() {
    float* worldScale = &appManager->info.worldScale;
    bool changedZoom = false;

    /*if (appManager->key[KEY_SCALE_UP]) {
        *worldScale += *worldScale * ZOOM_FACTOR;
        changedZoom = true;

    }
    else if (appManager->key[KEY_SCALE_DOWN]) {
        *worldScale -= *worldScale * ZOOM_FACTOR;
        changedZoom = true;
    }

    if (changedZoom) {
        calcScaledTile();
        calcScreenCapacity();
    }*/
}

///POSITION

void Camera::adjustPos(Point<float>& t_pos) {
    t_pos.x -= hitbox.x;
    t_pos.y -= hitbox.y;
}

void Camera::smoothPos(Point<float>& t_pos) {
    ///VARIABLES
    float pixelValue = 0;
    float remainder = 0;

    ///CALCULATE X
    pixelValue = 1.0f / scaledTile.w;

    remainder = fmod(t_pos.x, 1);
    remainder = fmod(remainder, pixelValue);
    t_pos.x -= remainder;

    if (remainder >= (pixelValue * 0.5)) {
        t_pos.x += pixelValue;
    }

    ///CALCULATE Y
    pixelValue = 1.0f / scaledTile.h;

    remainder = fmod(t_pos.y, 1);
    remainder = fmod(remainder, pixelValue);
    t_pos.y -= remainder;

    if (remainder >= (pixelValue * 0.5)) {
        t_pos.y += pixelValue;
    }
}

// SETTING / GETTING

void Camera::setFocus(Point<float> t_pos) {
    pos = t_pos;
}

void Camera::calcTileHitbox() {
    /// VARIABLES
    Hitbox<int> box;
    box.x = static_cast<int>(hitbox.x);
    box.y = static_cast<int>(hitbox.y);
    box.w = ceil(hitbox.w);
    box.h = ceil(hitbox.h);

    /// CHECK MAP BORDERS
    if (box.w > appManager->info.mapWidth) {
        box.w = appManager->info.mapWidth;
    }
    if (box.w < 0) {
        box.w = 0;
    }
    if (box.y > appManager->info.mapHeight) {
        box.y = appManager->info.mapHeight;
    }
    if (box.y < 0) {
        box.y = 0;
    }
    if (box.w < appManager->info.mapWidth) {
        box.w = appManager->info.mapWidth;
    }
    if (box.w > appManager->info.mapWidth - 1) {
        box.w = appManager->info.mapWidth - 1;
    }
    if (box.h < appManager->info.mapHeight) {
        box.h = appManager->info.mapHeight;
    }
    if (box.h > appManager->info.mapHeight - 1) {
        box.h = appManager->info.mapHeight - 1;
    }

    tileHitbox = box;
}

///CALCULATE

void Camera::calcHitbox() {
    ///SET
    Point<float> yLeft = this->pos;
    Point<float> downRight = this->pos;

    ///CALCULATE
    yLeft.x -= screenCapacity.w / 2;
    yLeft.y -= screenCapacity.h / 2;
    downRight.x += screenCapacity.w / 2;
    downRight.y += screenCapacity.h / 2;

    calcMapEnd();

    ///TOP LEFT BORDERS
    if (yLeft.x > mapEnd.w - screenCapacity.w) {
        yLeft.x = mapEnd.w - screenCapacity.w;
    }
    if (yLeft.y > mapEnd.h - screenCapacity.h) {
        yLeft.y = mapEnd.h - screenCapacity.h;
    }
    if (yLeft.x < 0) {
        yLeft.x = 0;
    }
    if (yLeft.y < 0) {
        yLeft.y = 0;
    }

    ///DOWN RIGHT BORDERS
    if (downRight.x < screenCapacity.w) {
        downRight.x = screenCapacity.w;
    }
    if (downRight.y < screenCapacity.h) {
        downRight.y = screenCapacity.h;
    }
    if (downRight.x > mapEnd.w) {
        downRight.x = mapEnd.w;
    }
    if (downRight.y > mapEnd.h) {
        downRight.y = mapEnd.h;
    }

    smoothPos(yLeft);

    hitbox.x = yLeft.x;
    hitbox.y = yLeft.y;
    hitbox.w = downRight.x;
    hitbox.h = downRight.y;
}

void Camera::calcScreenCapacity() {
    ///max tiles on screen
    screenCapacity.w = WINDOW_WIDTH / scaledTile.w;
    screenCapacity.h = WINDOW_HEIGHT / scaledTile.h;
}

void Camera::calcScaledTile() {
    scaledTile.w = rint(TILE_SIZE * appManager->info.worldScale);
    scaledTile.h = rint(TILE_SIZE * appManager->info.worldScale);
}

///INITIALIZE

void Camera::calcMapEnd() {
    mapEnd.w = appManager->info.mapWidth;
    mapEnd.h = appManager->info.mapHeight;
}
