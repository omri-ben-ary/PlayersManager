//
// Created by DELL on 27/11/2021.
//

#ifndef WET1_GROUP_H
#define WET1_GROUP_H
#include "Player.h"
#include "Node.h"
#include "AVLTree.h"

class Player;
class Group {
    int id;
    AVLTree* group_players_tree;

    static const int TEMP_VALUE;
public:
    Group();
    explicit Group(int id);
    ~Group() = default;
    Group(const Group& other) = default;
    Group& operator=(const Group& other) = default;

    int getId() const;
    AVLTree* getPlayersTree();
    void setPlayersTree(AVLTree* new_tree);
};
#endif //WET1_GROUP_H

