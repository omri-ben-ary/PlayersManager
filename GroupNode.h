#ifndef WET1_GROUPNODE_H
#define WET1_GROUPNODE_H
#include "Node.h"
#include "Group.h"

class GroupNode: public Node{
    Group* group;

public:
    explicit GroupNode(int id);
    GroupNode(int id, Group* new_group);
    ~GroupNode() override = default;
    GroupNode(const GroupNode& other) = default;
    GroupNode& operator=(const GroupNode& other) = default;
    Node* clone() override;
    int getExtraData() const override;  //returns group size
    Group* getGroup();
    void setGroup(Group* group_to_set);

    void updateExtraData(int index) override;
    int getSecondaryExtraData() const override;
    void copyExtraData(Node* other) override;
    int getHistogramData(int index) override;
    int getHistogramSum() override;
    int getThirdExtraData() const override;

};

#endif //WET1_GROUPNODE_H
