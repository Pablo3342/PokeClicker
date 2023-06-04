#pragma once

class Pokemon {
public:
    Pokemon(float t_health, float t_dmgPerTick);
    ~Pokemon();

    float health;
    float dmgPerTick;
    int bgID;

    bool encountered;
};