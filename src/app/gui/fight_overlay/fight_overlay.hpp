#pragma once

#include "../../app_manager/app_manager.hpp"
#include "../../../config/config.hpp"
#include "../../../struct/struct.hpp"

using namespace config;

class FightOverlay {
public:
    FightOverlay(AppManager* t_appManager);
    ~FightOverlay();

    std::vector<std::string> text;

    void setBulbasaur();
    void setPikatchu();
    void setPidgey();
    void setMeowth();
    void setSquirtle();
    void setPsyduck();
    void setShellder();

    void setStats(float t_pokemonHealth, float t_playerHealth);
    void statsTextPush();

    float pokemonHealth;
    float playerHealth;

private:
    AppManager* appManager;
};