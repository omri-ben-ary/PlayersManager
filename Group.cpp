#include "Group.h"
const int Group::TEMP_VALUE = 1;

Group::Group(int id): id(id) {
    group_players_tree = new AVLTree();
}

int Group::getId() const {
    return id;
}

AVLTree *Group::getPlayersTree() {
    return group_players_tree;
}

void Group::setPlayersTree(AVLTree *new_tree)
{
    delete this->group_players_tree;
    this->group_players_tree = new_tree;
}

Group::Group():id(TEMP_VALUE), group_players_tree(nullptr) {

}

