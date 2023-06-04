#pragma once

#include "../../config/config.hpp"
#include "../../struct/struct.hpp"
#include "timer/timer.hpp"

#include <functional>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#include <windows.h> // system cls

struct AppManagerInfo { // requires external initialization
    SDL_Event event;
    float worldScale = 0;
    bool gameLoop = false;

    int ticks2Process = 0;
    int frames2Render = 0;
    int allFramesRendered = 0;
    int allTicksProcessed = 0;

    int animation2Render = 0;
    int animationVecID = 0;

    DIALOG_BOX dialogBox = NONE;
    bool dialogPressed = false;
    bool lockPlayerMovement = false;
    int dialogBoxID = 0;

    bool attackPressed = false;
    bool doDamage = false;

    MAP map = SPAWN;

    int mapWidth = 0;
    int mapHeight = 0;

    EngineVector playerAccel = {0, 0};
};

struct AppManagerTimeInfo {
    double deltaTime = 0;
    double tickTimeLeft = 0;
    double frameTimeLeft = 0;
    double lastTimeStamp = 0;

    double animationDelta = 0;

    const double ANIMATION_AVG_TIME = 1 / config::ANIMATION_SPEED;
    const double FRAME_AVG_TIME = 1 / config::FPS;
    const double TICK_AVG_TIME = 1 / config::TPS;
};

enum KEY {
    MOVE_UP, MOVE_DOWN, MOVE_RIGHT, MOVE_LEFT, 
    SCALE_UP, SCALE_DOWN, SHOW_INFO, INTERACT, 
    ATTACK, KEY_NONE, ALL_KEYS
};

class AppManager {
public:
    AppManager();
    ~AppManager();

    AppManagerInfo info;

    void process();
    void startTimer();

    std::vector<PLAYER_ANIMATION>* currentPlayerAnimation;
    PLAYER_ANIMATION getCurrentAnimation();

private:
    std::function<void()> allEvents[ALL_KEYS];
    std::vector<KEY> keys;

    std::vector<PLAYER_ANIMATION> playerWalkDown;
    std::vector<PLAYER_ANIMATION> playerWalkUp;

    std::vector<PLAYER_ANIMATION>* previousPlayerAnimation;

    AppManagerTimeInfo timeInfo;
    Timer gameTimer;

    // process
    void processAnimation();
    void processUserInput();
    void processTime();

    // user processing
    KEY getKeyEvent(SDL_Event& event);
    void deleteEvent(KEY num);
    void addEvent(KEY num);

    void getUserInput();

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void interact();
    void attack();
    void doNothing();
    void showInfo();

    // calculations
    void calculateFrames2Render();
    void calculateTicks2Process();

    // init
    void initPlayerAnimations();
    void initAppManagerInfo();
    void initEvents();
};