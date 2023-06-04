#pragma once

#define PI 3.14159265359

namespace config {
    /// WINDOW
    constexpr int WINDOW_WIDTH = 1200;
    constexpr int WINDOW_HEIGHT = 800;
    constexpr const char* APP_TITLE = "PokeClicker";

    /// MAP
    constexpr int MAP_SPAWN_W = 20;
    constexpr int MAP_SPAWN_H = 20;

    constexpr int MAP_FOREST_W = 35;
    constexpr int MAP_FOREST_H = 35;

    constexpr int MAP_BEACH_W = 75;
    constexpr int MAP_BEACH_H = 25;
    
    constexpr int TILESHEET_W = 9;
    constexpr int TILESHEET_H = 16;
       
    constexpr int TILE_SIZE = 64; // in px

    // PLAYER
    constexpr float PLAYER_SPEED = 0.06f; // 0.05f; // 0.04
    constexpr float PLAYER_W = 0.99;
    constexpr float PLAYER_H = 0.99;

    constexpr int PLAYERSHEET_W = 2;
    constexpr int PLAYERSHEET_H = 2;

    constexpr int PLAYERSIZE_W = 52; // file size
    constexpr int PLAYERSIZE_H = 68; // file size

    constexpr double ANIMATION_SPEED = 3;

    // GENERATION
    constexpr float FULL_AREA = 0.1;
    constexpr int MAPS = 3;

    // GAME SPEED
    constexpr double TPS = 30;
    constexpr double FPS = 144;

    /// PHYSICS
    constexpr float FRICTION = 0.7f; // 0.82f // 0.9
    constexpr float PRECISION_MARGIN = 0.0001f;

    // RENDERING
    constexpr float WORLD_SCALE = 1.0f; // 0.5f

    constexpr int NEW_LINE = 512;
    constexpr int FONT_SIZE = 24;

    constexpr int DIALOG_BOX_X = WINDOW_WIDTH * 0.15;
    constexpr int DIALOG_BOX_Y = WINDOW_HEIGHT * 0.7;

    constexpr int DIALOG_BOX_W = WINDOW_WIDTH - DIALOG_BOX_X * 2;
    constexpr int DIALOG_BOX_H = WINDOW_HEIGHT - DIALOG_BOX_Y - 0.05 * WINDOW_HEIGHT;

    /// DEVELOPER OPTIONS
    constexpr bool PRECISION_SAFE_TIMER = true;
}