#include "Player.h"
Player::Player(int level) : level(level), player_group(nullptr) {}

bool Player::operator<(const Player &other) const {
    return (this->level < other.level);
}

bool Player::operator==(const Player &other) const {
    return (this->level == other.level);
}

int Player::getLevel() const {
    return level;
}

Group* Player::getGroup() {
    return player_group;
}

void Player::setGroup(Group* new_group) {
    player_group = new_group;
}

void Player::setLevel(int new_level) {
    level = new_level;
}

