#include "pokemon.hpp"

Pokemon::Pokemon(float t_health, float t_dmgPerTick)
    :   health(t_health),
        dmgPerTick(t_dmgPerTick),
        bgID(0),
        encountered(false)
{}

Pokemon::~Pokemon()
{}