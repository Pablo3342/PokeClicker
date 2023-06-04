#include "player.hpp"

Player::Player()
    :   pos({0,0}),
        box({0, 0, config::PLAYER_W, config::PLAYER_H }),
        accel({0,0}),
        vel({0,0}),
        health(100)
{}

Player::~Player()
{}

void Player::updateHitbox() {
    box.x = pos.x - 0.5 * box.w;
    box.y = pos.y - 0.5 * box.h;
}