#include "PlayerNode.h"

PlayerNode::PlayerNode(int id, int level, Group* group): Node(id), player(new Player(level)) {
    player->setGroup(group);
}

Node *PlayerNode::clone() {
    return new PlayerNode(*this);
}

int PlayerNode::getExtraData() const {
    return this->player->getLevel();
}

Player* PlayerNode::getPlayerInstance() {
    return this->player;
}

PlayerNode::PlayerNode(int id) : Node(id), player(nullptr) {

}

void PlayerNode::updateExtraData(int index) {

}

int PlayerNode::getSecondaryExtraData() const {
    return 0;
}

void PlayerNode::copyExtraData(Node *other) {

}

int PlayerNode::getHistogramData(int index) {
    return 0;
}

int PlayerNode::getHistogramSum() {
    return 0;
}

int PlayerNode::getThirdExtraData() const {
    return 0;
}


