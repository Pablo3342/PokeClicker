#include "app_manager.hpp"

AppManager::AppManager()
    :   gameTimer(),
        info(),
        timeInfo()
{
    initPlayerAnimations();
    initAppManagerInfo();
    initEvents();
}

AppManager::~AppManager()
{}

// PROCESS

void AppManager::process() {
    processTime();
    getUserInput();
    processUserInput();
    processAnimation();
}

PLAYER_ANIMATION AppManager::getCurrentAnimation() {
    if (currentPlayerAnimation == nullptr) {
        if (previousPlayerAnimation == nullptr) {
            return WALKING_UP_1;
        }
        return (*previousPlayerAnimation)[info.animationVecID];
    }
    return (*currentPlayerAnimation)[info.animationVecID];
}

void AppManager::processAnimation() {
    timeInfo.animationDelta += timeInfo.deltaTime;

    double remainder = fmod(timeInfo.animationDelta, timeInfo.ANIMATION_AVG_TIME);
    info.animation2Render = static_cast<int>((timeInfo.animationDelta - remainder) / timeInfo.ANIMATION_AVG_TIME);
    timeInfo.animationDelta = remainder;

    if (info.animation2Render > 0) {
        info.animation2Render = 0;
        if (currentPlayerAnimation == nullptr) {
            return;
        }

        const int SIZE = currentPlayerAnimation->size();

        ++info.animationVecID;

        if (info.animationVecID >= SIZE) {
            info.animationVecID = 0;
        }
    }
}

void AppManager::startTimer() {
    gameTimer.start();
}

// PROCESS TIME

void AppManager::processTime() {
    timeInfo.deltaTime = gameTimer.getTicks() - timeInfo.lastTimeStamp;
    timeInfo.frameTimeLeft += timeInfo.deltaTime;
    timeInfo.tickTimeLeft += timeInfo.deltaTime;

    calculateFrames2Render();
    calculateTicks2Process();

    timeInfo.lastTimeStamp = gameTimer.getTicks();

    if constexpr (config::PRECISION_SAFE_TIMER) {
        SDL_Delay(1); // precision safe timer
    }
}

void AppManager::calculateFrames2Render() {
    double remainder = fmod(timeInfo.frameTimeLeft, timeInfo.FRAME_AVG_TIME);
    info.frames2Render = static_cast<int>((timeInfo.frameTimeLeft - remainder) / timeInfo.FRAME_AVG_TIME);

    timeInfo.frameTimeLeft = remainder; /// keep for next processing
}

void AppManager::calculateTicks2Process() {
    double remainder = fmod(timeInfo.tickTimeLeft, timeInfo.TICK_AVG_TIME);
    info.ticks2Process = (timeInfo.tickTimeLeft - remainder) / timeInfo.TICK_AVG_TIME;    

    timeInfo.tickTimeLeft = remainder; /// keep for next processing
}

// USER INPUT

void AppManager::processUserInput() {
    info.playerAccel.x = 0;
    info.playerAccel.y = 0;

    for (auto& key : keys) {
        allEvents[key]();
    }
}

void AppManager::getUserInput() {

    while (SDL_PollEvent(&info.event)) {

        // exit program
        if (info.event.type == SDL_QUIT) {
            info.gameLoop = false;

        // process event input if key pressed
        } else if (info.event.type == SDL_KEYDOWN) {
            addEvent(getKeyEvent(info.event));

        } else if (info.event.type == SDL_KEYUP) {
            deleteEvent(getKeyEvent(info.event));
            info.dialogPressed = false;
            info.attackPressed = false;
        }
    }
}

KEY AppManager::getKeyEvent(SDL_Event& event) {

    switch (event.key.keysym.sym) {
        case 'q':
            return SCALE_DOWN;
        break;

        case 'e':
            return SCALE_UP;
        break;

        case 'i':
            return SHOW_INFO;
        break;

        case 'w':
            return MOVE_UP;
        break;

        case 's':
            return MOVE_DOWN;
        break;

        case 'a':
            return MOVE_LEFT;
        break;

        case 'd':
            return MOVE_RIGHT;
        break;

        case 'z':
            return INTERACT;
        break;

        case ' ':
            return ATTACK;
        break;
    }

    return KEY_NONE;
}

void AppManager::deleteEvent(KEY key) {
    std::function<void(void)> sample;
    const int SIZE = keys.size();

    for (int i = 0; i < SIZE; ++i) {

        if (keys[i] == key) {
            keys[i] = keys.back();
            keys.pop_back();
            return;
        }
    }
}

void AppManager::addEvent(KEY key) {
    for (auto& inside : keys) {
        if (inside == key) {
            return;
        }
    }

    keys.push_back(key);
}

void AppManager::showInfo() {
    system("CLS");
    printf(" - - DEV  INFO - - \n");
    printf("Time running: %f\n", gameTimer.getTimeSinceStart());
    printf("FPS: %f\n", info.allFramesRendered / gameTimer.getTimeSinceStart());
    printf("TPS: %f\n", info.allTicksProcessed / gameTimer.getTimeSinceStart());
    printf(" - - - - - - - - - \n");
}

void AppManager::moveUp() {
    if (info.lockPlayerMovement) {
        return;
    }
    info.playerAccel.y -= 1;

    if (currentPlayerAnimation != &playerWalkUp) {
        currentPlayerAnimation = &playerWalkUp;
        previousPlayerAnimation = currentPlayerAnimation;
        info.animationVecID = 0;
    }
}

void AppManager::moveDown() {
    if (info.lockPlayerMovement) {
        return;
    }
    info.playerAccel.y += 1;

    if (currentPlayerAnimation != &playerWalkDown) {
        currentPlayerAnimation = &playerWalkDown;
        previousPlayerAnimation = currentPlayerAnimation;
        info.animationVecID = 0;
    }
}

void AppManager::moveLeft() {
    if (info.lockPlayerMovement) {
        return;
    }
    info.playerAccel.x -= 1;
    currentPlayerAnimation = previousPlayerAnimation;
}

void AppManager::moveRight() {
    if (info.lockPlayerMovement) {
        return;
    }
    info.playerAccel.x += 1;
    currentPlayerAnimation = previousPlayerAnimation;
}

void AppManager::interact() {
    if (info.dialogPressed) {
        return;
    }
    info.dialogPressed = true;

    if (info.dialogBox != NONE) {
        ++info.dialogBoxID;
    }
}

void AppManager::attack() {
    if (info.attackPressed) {
        return;
    }
    info.attackPressed = true;
    info.doDamage = true;
}

void AppManager::doNothing() {

}

// INITIALIZE

void AppManager::initPlayerAnimations() {
    playerWalkDown.push_back(WALKING_UP_1);
    playerWalkDown.push_back(WALKING_UP_2);

    playerWalkUp.push_back(WALKING_DOWN_1);
    playerWalkUp.push_back(WALKING_DOWN_2);

    currentPlayerAnimation = nullptr;
    previousPlayerAnimation = nullptr;
}

void AppManager::initAppManagerInfo() {
    info.worldScale = config::WORLD_SCALE;
    info.gameLoop = true;

    info.dialogBox = PROFFESOR;
    info.lockPlayerMovement = true;
    info.dialogBoxID = 0;

    info.mapWidth = config::MAP_SPAWN_W;
    info.mapHeight = config::MAP_SPAWN_H;

    info.map = SPAWN;
}

void AppManager::initEvents() {
    // movement
    allEvents[MOVE_UP] = std::bind(&AppManager::moveUp, this);
    allEvents[MOVE_DOWN] = std::bind(&AppManager::moveDown, this);
    allEvents[MOVE_RIGHT] = std::bind(&AppManager::moveRight, this);
    allEvents[MOVE_LEFT] = std::bind(&AppManager::moveLeft, this);
    
    // scale
    allEvents[SCALE_UP] = std::bind(&AppManager::doNothing, this);
    allEvents[SCALE_DOWN] = std::bind(&AppManager::doNothing, this);

    // functon keys
    allEvents[INTERACT] = std::bind(&AppManager::interact, this);
    allEvents[ATTACK] = std::bind(&AppManager::attack, this);
    allEvents[SHOW_INFO] = std::bind(&AppManager::showInfo, this);

    // none "placeholder"
    allEvents[KEY_NONE] = std::bind(&AppManager::doNothing, this);
}