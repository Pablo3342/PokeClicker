#pragma once

#include "../../app_manager/app_manager.hpp"
#include "../../../config/config.hpp"
#include "../../../struct/struct.hpp"

#include <cmath>

using namespace config;

class Camera {
public:
    Camera(AppManager* t_appManager);
    ~Camera();

    ///VARIABLES
    AppManager* appManager;
    Hitbox<int> tileHitbox;
    Hitbox<float> hitbox;

    ///PROCESS
    void process();
    void processZoom();

    ///POSITION
    void adjustPos(Point<float>& t_pos);//adjust given object to camera
    void smoothPos(Point<float>& t_pos);//round position to nearest pixel position

    ///SETTING / GETTING
    void setFocus(Point<float> t_pos);

public:
    ///VARIABLES
    Size<float> screenCapacity;//how many tiles can be once at screen
    Size<float> scaledTile;
    Size<float> mapEnd;
    Point<float> pos;

    ///CALCULATE
    void calcTileHitbox();
    void calcHitbox();

    void calcScreenCapacity();
    void calcScaledTile();

    ///INITIALIZE
    void calcMapEnd();
};