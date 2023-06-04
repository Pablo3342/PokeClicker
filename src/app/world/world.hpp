#pragma once

#include "../app_manager/app_manager.hpp"
#include "../gui/fight_overlay/fight_overlay.hpp"
#include "level/level.hpp"
#include "../../config/config.hpp"
#include "random/random.hpp"
#include "player/player.hpp"
#include "pokemon/pokemon.hpp"

using namespace config;

enum TELEPORT {
    SPAWN_TELEPORT, FOREST_TELEPORT, BEACH_TELEPORT, NO_TELEPORT
};

struct PokemonInfo {
    int id = 0;
    int x = 0;
    int y = 0;
};

class World {
public:
    World(AppManager* t_appManager);
    ~World();

    void process();
    void processTick();

    void processPlayer();

    Level<MAP_SPAWN_W, MAP_SPAWN_H> spawn;
    Level<MAP_FOREST_W, MAP_FOREST_H> forest;
    Level<MAP_BEACH_W, MAP_BEACH_H> beach;

    std::unordered_map<std::string, int> tiles; // just to sort things out

    std::vector<int> pokemonTiles;
    std::vector<int> wallTiles;
    std::vector<int> forestTeleport;
    std::vector<int> spawnTeleport;
    std::vector<int> beachTeleport;

    Player player;
    FightOverlay fightOverlay;

private:
    Random random;
    AppManager* appManager;

    void movePlayer(Point<float> newPos);

    Point<float> handlePlayerWallCollision(); // gives new player position
    std::vector<Point<int>> getFutureWallsCollision(const float& velX, const float& velY);

    bool checkCollision(const Hitbox<float>& rect1, const Hitbox<float>& rect2);

    void handlePokemonFight(PokemonInfo info);
    PokemonInfo checkPokemonCollision();

    TELEPORT checkTeleportTiles();
    void teleportPlayerSpawn();
    void teleportPlayerForest(int path);
    void teleportPlayerBeach(int path);

    // forest
    Pokemon bulbasaur;
    Pokemon pikatchu;
    Pokemon pidgey;
    Pokemon meowth;

    // beach
    Pokemon squirtle;
    Pokemon psyduck;
    Pokemon shellder;

    void initPokemons();
    void setTiles();
    void preinit();
};