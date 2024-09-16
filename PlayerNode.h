#ifndef WET1_PLAYERNODE_H
#define WET1_PLAYERNODE_H
#include "Node.h"
#include "Player.h"


class PlayerNode: public Node{
    Player* player;

public:
    explicit PlayerNode(int id);
    PlayerNode(int id, int level, Group* group);
    ~PlayerNode() override = default;
    PlayerNode(const PlayerNode& other) = default;
    PlayerNode& operator=(const PlayerNode& other) = default;
    Node* clone() override;
    int getExtraData() const override;  //returns player level
    Player* getPlayerInstance();

    void updateExtraData(int index) override;
    int getSecondaryExtraData() const override;
    void copyExtraData(Node* other) override;
    int getHistogramData(int index) override;
    int getHistogramSum() override;
    int getThirdExtraData() const override;
};
#endif //WET1_PLAYERNODE_H
