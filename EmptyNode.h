#ifndef WET1_EMPTYNODE_H
#define WET1_EMPTYNODE_H
#include "Node.h"
class EmptyNode: public Node{
public:
    EmptyNode();
    ~EmptyNode() override = default;
    EmptyNode(const EmptyNode& other) = default;
    EmptyNode& operator=(const EmptyNode& other) = default;
    Node* clone() override;
    int getExtraData() const override;
    void updateExtraData(int index) override;
    int getSecondaryExtraData() const override;
    void copyExtraData(Node* other) override;
    int getHistogramData(int index) override;
    int getHistogramSum() override;
    int getThirdExtraData() const override;
};
#endif //WET1_EMPTYNODE_H

