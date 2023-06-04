#include "fight_overlay.hpp"

FightOverlay::FightOverlay(AppManager* t_appManager)
    :   appManager(t_appManager),
        pokemonHealth(0),
        playerHealth(0)
{

}

FightOverlay::~FightOverlay()
{}

void FightOverlay::setBulbasaur() {
    text.clear();
    text.push_back("Wild Bulbasaur is attacking you!");
    text.push_back("Fight!");
}

void FightOverlay::setPikatchu() {
    text.clear();
    text.push_back("Wild Pikatchu is attacking you!");
    text.push_back("Fight!");
}

void FightOverlay::setPidgey() {
    text.clear();
    text.push_back("Wild Pidgey is attacking you!");
    text.push_back("Fight!");
}

void FightOverlay::setMeowth() {
    text.clear();
    text.push_back("Wild Meowth is attacking you!");
    text.push_back("Fight!");
}

void FightOverlay::setSquirtle() {
    text.clear();
    text.push_back("Wild Squirtle is attacking you!");
    text.push_back("Fight!");
}

void FightOverlay::setPsyduck() {
    text.clear();
    text.push_back("Wild Psyduck is attacking you!");
    text.push_back("Fight!");
}

void FightOverlay::setShellder() {
    text.clear();
    text.push_back("Wild Shellder is attacking you!");
    text.push_back("Fight!");
}

void FightOverlay::setStats(float t_pokemonHealth, float t_playerHealth) {
    pokemonHealth = t_pokemonHealth;
    playerHealth = t_playerHealth;
}

void FightOverlay::statsTextPush() {
    std::string msg;
    msg += "Player health: " + std::to_string(playerHealth) + "                      ";
    msg += "Pokemon health: " + std::to_string(pokemonHealth) + "                     ";
    
    text.pop_back(); 
    text.push_back(msg);
}