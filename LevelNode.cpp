#include "LevelNode.h"

LevelNode::LevelNode(int id, Player* player): Node(id), player(player), histogram(), sum(0) {
    histogram[0]++;
    sum = getPlayerInstance()->getLevel();
}
/*
LevelNode::LevelNode(const LevelNode& other): Node(other.getNodeKey()), player(other.player), histogram() {
    for(int i = 0; i < MAX_SCALE; i++)
    {
        histogram[i] = other.histogram[i];
    }
}

LevelNode& LevelNode::operator=(const LevelNode& other){
    if(&other == this)
    {
        return *this;
    }
    this->setNodeKey(other.getNodeKey());
    this->setNodeHeight(other.getNodeHeight());
    this->setNodeBF(other.getNodeBF());
    this->setNodeLeftSon(other.getNodeLeftSon());
    this->setNodeRightSon(other.getNodeRightSon());
    this->setNodeParent(other.getNodeParent());
    for(int i = 0; i < MAX_SCALE; i++)
    {
        histogram[i] = other.histogram[i];
    }
    return *this;
}*/

Node *LevelNode::clone(){
    return new LevelNode(*this);
}

bool LevelNode::operator<(const Node& second_node) const {
    if(this->getExtraData() == second_node.getExtraData())
        return (second_node.getNodeKey() < this->getNodeKey());
    return (this->getExtraData() < second_node.getExtraData());
}

int LevelNode::getExtraData() const {
    return this->player->getLevel();
}

bool LevelNode::operator==(const Node &second_node) const {
    return ((this->getNodeKey() == second_node.getNodeKey()) && (this->getExtraData() == second_node.getExtraData()));
}

Player* LevelNode::getPlayerInstance() const {
    return this->player;
}

void LevelNode::updateExtraData(int index) {
    LevelNode *left_son = nullptr, *right_son = nullptr;

    if (getNodeLeftSon() != nullptr) {
        left_son = dynamic_cast<LevelNode *>(getNodeLeftSon());
    }
    if (getNodeRightSon() != nullptr) {
        right_son = dynamic_cast<LevelNode *>(getNodeRightSon());
    }

    for(int i = 0; i < MAX_SCALE; i++)
    {
        int left_son_histogram_value = 0, right_son_histogram_value = 0;
        if (left_son != nullptr) {
            left_son_histogram_value = left_son->histogram[i];
        }

        if (right_son != nullptr) {
            right_son_histogram_value = right_son->histogram[i];
        }
        histogram[i] = left_son_histogram_value + right_son_histogram_value;
    }
    int left_son_sum = 0 , right_son_sum = 0;
    if(left_son != nullptr)
    {
        left_son_sum = left_son->sum;
    }
    if(right_son != nullptr)
    {
        right_son_sum = right_son->sum;
    }
    sum = left_son_sum + right_son_sum + getPlayerInstance()->getLevel();
    histogram[index]++;
}

int LevelNode::getSecondaryExtraData() const {
    return 0;
}

void LevelNode::copyExtraData(Node* other) {
    LevelNode* level_other = dynamic_cast<LevelNode*>(other);
    for(int i = 0; i < MAX_SCALE; i++)
    {
        histogram[i] = level_other->histogram[i];
    }
}

int LevelNode::getHistogramData(int index) {
    return histogram[index];
}

int LevelNode::getHistogramSum() {
    int sum = 0;
    for(int i = 0; i < MAX_SCALE; i++)
    {
        sum += histogram[i];
    }
    return sum;
}

int LevelNode::getThirdExtraData() const {
    return sum;
}
