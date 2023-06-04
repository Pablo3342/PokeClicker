#pragma once

#include "../../app_manager/app_manager.hpp"

class Player {
public:
    Player();
    ~Player();

    void updateHitbox();

    float health;
    Point<float> pos;

    Hitbox<float> box;

    EngineVector accel;
    EngineVector vel;
};