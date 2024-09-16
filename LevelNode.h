#ifndef WET1_LEVELNODE_H
#define WET1_LEVELNODE_H

#include "Node.h"
#include "Player.h"

class LevelNode: public Node{
    Player* player;
    static const int MAX_SCALE = 201;
    int histogram[MAX_SCALE];
    int sum;
public:
    LevelNode(int id, Player* player);
    ~LevelNode() override = default;
    LevelNode(const LevelNode& other) = default;
    LevelNode& operator=(const LevelNode& other) = default;
    Node* clone() override;
    bool operator<(const Node& second_node) const override;
    bool operator==(const Node& second_node) const override;
    Player* getPlayerInstance() const;

    int getExtraData() const override;  //returns player level
    int getSecondaryExtraData() const override; //returns player score
    int getThirdExtraData() const override; //returns sum
    int getHistogramData(int index) override;
    int getHistogramSum() override;
    void updateExtraData(int index) override;   //updates histogram and sum
    void copyExtraData(Node* other) override;   //maybe delete
};
#endif //WET1_LEVELNODE_H
