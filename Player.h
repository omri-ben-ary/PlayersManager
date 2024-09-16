//
// Created by DELL on 27/11/2021.
//

#ifndef WET1_PLAYER_H
#define WET1_PLAYER_H

class Group;
class Player{
    int level;
    Group* player_group;
public:
    explicit Player(int level);
    ~Player() = default;
    Player(const Player& other) = default;
    Player& operator=(const Player& other) = default;

    bool operator<(const Player& other) const;
    bool operator==(const Player& other) const;
    int getLevel() const;
    void setLevel(int new_level);
    Group* getGroup();
    void setGroup(Group* new_group);
};
#endif //WET1_PLAYER_H

