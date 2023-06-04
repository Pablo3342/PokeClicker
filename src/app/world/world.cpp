#include "world.hpp"

World::World(AppManager* t_appManager)
    :   spawn("maps/spawn.csv"),
        forest("maps/forest.csv"),
        beach("maps/beach.csv"),
        fightOverlay(t_appManager),
        player(),
        random(),
        appManager(t_appManager),
        bulbasaur(0, 0),
        pikatchu(0, 0),
        pidgey(0, 0),
        meowth(0, 0),
        squirtle(0, 0),
        psyduck(0, 0),
        shellder(0, 0)
{
    preinit();
}

World::~World() 
{}

void World::process() {
    for (int i = 0; i < appManager->info.ticks2Process; ++i) {
        ++appManager->info.allTicksProcessed;
        processTick();
    }
}

void World::processTick() {
    processPlayer();
}

void World::processPlayer() {
    // temp
    //std::cout << player.pos.x << ' ' << player.pos.y << '\n';

    // check if dead
    if (player.health <= 0) {
        appManager->info.dialogBox = NONE;
        appManager->info.lockPlayerMovement = false;
        player.health = 100;
        teleportPlayerSpawn();
    }

    // get input from appmanager and convert to unitvector
    player.accel = appManager->info.playerAccel;
    float tempMag = pow(pow(player.accel.x, 2) + pow(player.accel.y, 2), 0.5);
    if (tempMag > 0) {
        player.accel.x = player.accel.x * PLAYER_SPEED / tempMag;
        player.accel.y = player.accel.y * PLAYER_SPEED / tempMag;
    }

    // apply accel 2 velocity
    player.vel.x = player.vel.x + player.accel.x;
    player.vel.y = player.vel.y + player.accel.y;
    
    // friction
    player.vel.x *= FRICTION;
    player.vel.y *= FRICTION;

    // teleport handling
    switch (checkTeleportTiles()) {
        case SPAWN_TELEPORT:
            teleportPlayerSpawn();
            return;
        break;

        case FOREST_TELEPORT:
            if (appManager->info.map == SPAWN) {
                teleportPlayerForest(0);
            }
            if (appManager->info.map == BEACH) {
                teleportPlayerForest(1);
            }
            return;
        break;

        case BEACH_TELEPORT:
            if (appManager->info.map == SPAWN) {
                teleportPlayerBeach(0);
            }
            if (appManager->info.map == FOREST) {
                teleportPlayerBeach(1);
            }
            return;
        break;
    }
    
    Point<float> newPos = handlePlayerWallCollision();

    // assin new position based on collision
    if (newPos.x != -1) {
        player.pos.x = newPos.x;
    } else {
        player.pos.x += player.vel.x;
    }
    if (newPos.y != -1) {
        player.pos.y = newPos.y;
    } else {
        player.pos.y += player.vel.y;
    }
    player.updateHitbox();

    // handle pokemon interaction
    PokemonInfo pokemonInfo = checkPokemonCollision();

    if (pokemonInfo.id != -1) {
        handlePokemonFight(pokemonInfo);
    }

    if (player.vel.x >= -config::PRECISION_MARGIN &&
        player.vel.x <= config::PRECISION_MARGIN && 
        player.vel.y >= -config::PRECISION_MARGIN &&
        player.vel.y <= config::PRECISION_MARGIN)
    {
        appManager->currentPlayerAnimation = nullptr;
    }
    
    // border check
    if (player.pos.x - 0.5 * player.box.w < 0) {
        player.pos.x = 0.5 * player.box.w;
    }
    else if (player.pos.x + 0.5 * player.box.w >= appManager->info.mapWidth - PRECISION_MARGIN) {
        player.pos.x = appManager->info.mapWidth - 0.5 * player.box.w - PRECISION_MARGIN;
    }
    if (player.pos.y - 0.5 * player.box.h < 0) {
        player.pos.y = 0.5 * player.box.h;
    }
    else if (player.pos.y + 0.5 * player.box.h >= appManager->info.mapHeight - PRECISION_MARGIN) {
        player.pos.y = appManager->info.mapHeight - 0.5 * player.box.h - PRECISION_MARGIN;
    }
    player.updateHitbox();
}

Point<float> World::handlePlayerWallCollision() {
    std::vector<Point<int>> walls;

    Hitbox<float> wall = {0, 0, 1, 1};
    Point<float> newPos = { -1, -1 };

    // check X axis
    walls = getFutureWallsCollision(player.vel.x, 0);

    if (walls.size() > 0) {

        wall.x = walls.back().x;

        if (player.vel.x > 0) {
            newPos.x = wall.x - player.box.w * 0.5 - config::PRECISION_MARGIN;
        }
        else if (player.vel.x < 0) {
            newPos.x = wall.x + wall.w + player.box.w * 0.5 + config::PRECISION_MARGIN;
        }
    }

    walls.clear();

    // check Y axis
    walls = getFutureWallsCollision(0, player.vel.y);

    if (walls.size() > 0) {

        wall.y = walls.back().y;

        if (player.vel.y > 0) {
            newPos.y = wall.y - player.box.h * 0.5 - config::PRECISION_MARGIN;
        }
        else if (player.vel.y < 0) {
            newPos.y = wall.y + wall.h + player.box.h * 0.5 + config::PRECISION_MARGIN;
        }
    }

    return newPos;
}

std::vector<Point<int>> World::getFutureWallsCollision(const float& velX, const float& velY) {
    Point<float> nextPos = player.pos;
    nextPos.x += velX;
    nextPos.y += velY;

    Hitbox<float> nextBox;
    nextBox.x = nextPos.x - 0.5 * player.box.w;
    nextBox.y = nextPos.y - 0.5 * player.box.h;
    nextBox.w = player.box.w;
    nextBox.h = player.box.h;

    int x = static_cast<int>(nextBox.x);
    int y = static_cast<int>(nextBox.y);
    int w = static_cast<int>(nextBox.x + nextBox.w);
    int h = static_cast<int>(nextBox.y + nextBox.h);

    std::vector<Point<int>> walls;

    for (int i = y; i <= h; ++i) {
        for (int j = x; j <= w; ++j) {
            for (auto& wall : wallTiles) {
                if (appManager->info.map == SPAWN) {
                    if (spawn.map[i][j] == wall) {
                        walls.push_back({ j, i });
                    }
                }
                else if (appManager->info.map == FOREST) {
                    if (forest.map[i][j] == wall) {
                        walls.push_back({ j, i });
                    }
                }
                else if (appManager->info.map == BEACH) {
                    if (beach.map[i][j] == wall) {
                        walls.push_back({ j, i });
                    }
                }
            }
        }
    }

    return walls;
}

void World::handlePokemonFight(PokemonInfo info) {
    appManager->info.lockPlayerMovement = true;
    appManager->info.dialogBox = FIGHT;

    Pokemon* pokemon = nullptr;

    if (info.id == tiles["bulbasaur"]) {
        pokemon = &bulbasaur;
        if (!pokemon->encountered) {
            fightOverlay.setBulbasaur();
        }
    }
    if (info.id == tiles["pikatchu"]) {
        pokemon = &pikatchu;
        if (!pokemon->encountered) {
            fightOverlay.setPikatchu();
        }
    }
    if (info.id == tiles["pidgey"]) {
        pokemon = &pidgey;
        if (!pokemon->encountered) {
            fightOverlay.setPidgey();
        }
    }
    if (info.id == tiles["meowth"]) {
        pokemon = &meowth;
        if (!pokemon->encountered) {
            fightOverlay.setMeowth();
        }
    }
    if (info.id == tiles["squirtle"]) {
        pokemon = &squirtle;
        if (!pokemon->encountered) {
            fightOverlay.setSquirtle();
        }
    }
    if (info.id == tiles["psyduck"]) {
        pokemon = &psyduck;
        if (!pokemon->encountered) {
            fightOverlay.setPsyduck();
        }
    }
    if (info.id == tiles["shellder"]) {
        pokemon = &shellder;
        if (!pokemon->encountered) {
            fightOverlay.setShellder();
        }
    }

    if (pokemon == nullptr) {
        return;
    }

    if (!pokemon->encountered) {
        pokemon->encountered = true;
    }

    player.vel.x = 0;
    player.vel.y = 0;

    if (appManager->info.dialogBoxID != fightOverlay.text.size() -1) {
        return;
    }

    player.health -= pokemon->dmgPerTick;

    if (appManager->info.doDamage) {
        pokemon->health -= random.getReal<float>(1, 2);
        appManager->info.doDamage = false;
    }

    // win
    if (pokemon->health <= 0) {
        if (appManager->info.map == SPAWN) {
            spawn.map[info.y][info.x] = pokemon->bgID;
        }
        else if (appManager->info.map == FOREST) {
            forest.map[info.y][info.x] = pokemon->bgID;
        }
        else if (appManager->info.map == BEACH) {
            beach.map[info.y][info.x] = pokemon->bgID;
        }
        appManager->info.dialogBox = NONE;
        appManager->info.dialogBoxID = 0;
        appManager->info.lockPlayerMovement = false;
        player.health = 100;
    }

    fightOverlay.setStats(pokemon->health, player.health);
    fightOverlay.statsTextPush();
}

void World::teleportPlayerSpawn() {
    player.pos.x = 10;
    player.pos.y = 14.5;
    player.vel.x = 0;
    player.vel.y = 0;

    appManager->info.map = SPAWN;
    appManager->info.mapWidth = config::MAP_SPAWN_W;
    appManager->info.mapHeight = config::MAP_SPAWN_H;
}

void World::teleportPlayerForest(int path) {
    if (path == 0) {
        player.pos.x = 2;
        player.pos.y = 7;
    }
    else if (path == 1) {
        player.pos.x = 28;
        player.pos.y = 32.5;
    }

    player.vel.x = 0;
    player.vel.y = 0;

    appManager->info.map = FOREST;
    appManager->info.mapWidth = config::MAP_FOREST_W;
    appManager->info.mapHeight = config::MAP_FOREST_H;
}

void World::teleportPlayerBeach(int path) {
    if (path == 0) {
        player.pos.x = 11.5;
        player.pos.y = 3.5;
    }
    else if (path == 1) {
        player.pos.x = 73;
        player.pos.y = 5.5;
    }
    
    player.vel.x = 0;
    player.vel.y = 0;

    appManager->info.map = BEACH;
    appManager->info.mapWidth = config::MAP_BEACH_W;
    appManager->info.mapHeight = config::MAP_BEACH_H;
}

PokemonInfo World::checkPokemonCollision() {
    Hitbox<float> nextBox;
    nextBox.x = player.pos.x - 0.5 * player.box.w;
    nextBox.y = player.pos.y - 0.5 * player.box.h;
    nextBox.w = player.box.w;
    nextBox.h = player.box.h;

    int x = static_cast<int>(nextBox.x);
    int y = static_cast<int>(nextBox.y);
    int w = static_cast<int>(nextBox.x + nextBox.w);
    int h = static_cast<int>(nextBox.y + nextBox.h);

    PokemonInfo info;

    for (int i = y; i <= h; ++i) {
        for (int j = x; j <= w; ++j) {
            for (auto& pokemon : pokemonTiles) {
                if (appManager->info.map == SPAWN) {
                    if (spawn.map[i][j] == pokemon) {
                        info.id = pokemon;
                        info.x = j;
                        info.y = i;
                        return info;
                    }
                }
                if (appManager->info.map == FOREST) {
                    if (forest.map[i][j] == pokemon) {
                        info.id = pokemon;
                        info.x = j;
                        info.y = i;
                        return info;
                    }
                }
                if (appManager->info.map == BEACH) {
                    if (beach.map[i][j] == pokemon) {
                        info.id = pokemon;
                        info.x = j;
                        info.y = i;
                        return info;
                    }
                }
            }
        }
    }

    return {-1, -1, -1};
}

TELEPORT World::checkTeleportTiles() {
    Hitbox<float> nextBox;
    nextBox.x = player.pos.x - 0.5 * player.box.w;
    nextBox.y = player.pos.y - 0.5 * player.box.h;
    nextBox.w = player.box.w;
    nextBox.h = player.box.h;

    int x = static_cast<int>(nextBox.x);
    int y = static_cast<int>(nextBox.y);
    int w = static_cast<int>(nextBox.x + nextBox.w);
    int h = static_cast<int>(nextBox.y + nextBox.h);

    for (int i = y; i <= h; ++i) {
        for (int j = x; j <= w; ++j) {
            if (appManager->info.map == SPAWN) {
                for (auto& tile : spawnTeleport) {
                    if (spawn.map[i][j] == tile) {
                        return SPAWN_TELEPORT;
                    }
                }
                for (auto& tile : forestTeleport) {
                    if (spawn.map[i][j] == tile) {
                        return FOREST_TELEPORT;
                    }
                }
                for (auto& tile : beachTeleport) {
                    if (spawn.map[i][j] == tile) {
                        return BEACH_TELEPORT;
                    }
                }
            }
            if (appManager->info.map == FOREST) {
                for (auto& tile : spawnTeleport) {
                    if (forest.map[i][j] == tile) {
                        return SPAWN_TELEPORT;
                    }
                }
                for (auto& tile : forestTeleport) {
                    if (forest.map[i][j] == tile) {
                        return FOREST_TELEPORT;
                    }
                }
                for (auto& tile : beachTeleport) {
                    if (forest.map[i][j] == tile) {
                        return BEACH_TELEPORT;
                    }
                }
            }
            if (appManager->info.map == BEACH) {
                for (auto& tile : spawnTeleport) {
                    if (beach.map[i][j] == tile) {
                        return SPAWN_TELEPORT;
                    }
                }
                for (auto& tile : forestTeleport) {
                    if (beach.map[i][j] == tile) {
                        return FOREST_TELEPORT;
                    }
                }
                for (auto& tile : beachTeleport) {
                    if (beach.map[i][j] == tile) {
                        return BEACH_TELEPORT;
                    }
                }
            }
        }
    }

    return NO_TELEPORT;
}

bool World::checkCollision(const Hitbox<float>& rect1, const Hitbox<float>& rect2) {
    float d1x = rect1.x - (rect2.x + rect2.w);
    float d1y = rect1.y - (rect2.y + rect2.h);
    float d2x = rect2.x - (rect1.x + rect1.w);
    float d2y = rect2.y - (rect1.y + rect1.h);

    if (d1x > 0.0f || d1y > 0.0f) {
        return false;
    }

    if (d2x > 0.0f || d2y > 0.0f) {
        return false;
    }

    return true;
}

void World::movePlayer(Point<float> newPos) {
    player.pos.x = newPos.x;
    player.pos.y = newPos.y;
}

void World::setTiles() {
    int array[] = { 0, 1, 6, 7, 14, 15, 16, 19, 20, 36, 37, 45, 46, 
        54, 55, 56, 63, 64, 65, 67, 68, 69, 70, 72, 73, 74, 76, 77, 78, 
        79, 81, 82, 83, 85, 86, 87, 88, 90, 91, 92, 93, 94, 95, 96, 97, 
        98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 111, 112, 113, 
        114, 115, 116, 130, 131, 139, 140 };

    for (auto& num : array) {
        wallTiles.push_back(num);
    }

    // set pokemon tiles
    tiles["bulbasaur"] = 44;
    pokemonTiles.push_back(44);

    tiles["pikatchu"] = 120;
    pokemonTiles.push_back(120);

    tiles["pidgey"] = 62;
    pokemonTiles.push_back(62);

    tiles["meowth"] = 53;
    pokemonTiles.push_back(53);

    tiles["squirtle"] = 26;
    pokemonTiles.push_back(26);

    tiles["psyduck"] = 8;
    pokemonTiles.push_back(8);

    tiles["shellder"] = 138;
    pokemonTiles.push_back(138);
}

void World::preinit() {
    appManager->info.mapWidth = config::MAP_SPAWN_W;
    appManager->info.mapHeight = config::MAP_SPAWN_H;

    tiles["grass"] = 38;
    tiles["sand"] = 13;

    setTiles();
    
    spawnTeleport.push_back(123);
    spawnTeleport.push_back(125);

    forestTeleport.push_back(39);
    forestTeleport.push_back(124);

    beachTeleport.push_back(118);
    beachTeleport.push_back(129);

    initPokemons();

    teleportPlayerSpawn();
}

void World::initPokemons() {
    bulbasaur.health = random.getInt(90, 150);
    bulbasaur.dmgPerTick = random.getReal(0.12, 0.3);
    bulbasaur.bgID = tiles["grass"];

    pikatchu.health = random.getInt(70, 120);
    pikatchu.dmgPerTick = random.getReal(0.08, 0.6);
    pikatchu.bgID = tiles["grass"];

    meowth.health = random.getInt(90, 150);
    meowth.dmgPerTick = random.getReal(0.08, 0.3);
    meowth.bgID = tiles["grass"];

    pidgey.health = random.getInt(90, 150);
    pidgey.dmgPerTick = random.getReal(0.08, 0.3);
    pidgey.bgID = tiles["grass"];

    squirtle.health = random.getInt(90, 150);
    squirtle.dmgPerTick = random.getReal(0.09, 0.3);
    squirtle.bgID = tiles["sand"];

    psyduck.health = random.getInt(120, 180);
    psyduck.dmgPerTick = random.getReal(0.12, 0.2);
    psyduck.bgID = tiles["sand"];

    shellder.health = random.getInt(80, 120);
    shellder.dmgPerTick = random.getReal(0.06, 0.2);
    shellder.bgID = tiles["sand"];
}