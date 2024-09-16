#ifndef WET1_NODE_H
#define WET1_NODE_H
class Node {
private:
    int key;
    int height;
    int balance_factor;
    Node* left_son; // when do i allocate memory for it -  by the manager
    Node* right_son; // when do i allocate memory for it - by the manager
    Node* parent; // when do i allocate memory for it - by the manager
    static const int HEIGHT_OF_LEAF;


public:
    explicit Node(int key);
    Node(const Node&) = default; // is there a reason for me that after making a placement the two nodes will point to different places
    Node& operator=(const Node&) = default; // is there a reason for me that after copying a node the two nodes will point to different places
    virtual ~Node() = default; // need to check this one - maybe should be something else (because it might free pointers before we want it to do so)
    virtual Node* clone() = 0; // need to check this one - maybe should be something else
    virtual bool operator<(const Node& second_node) const; // probably need to change Node* to node
    bool operator>(const Node& second_node) const; // probably need to change Node* to node
    bool operator>=(const Node& second_node) const;
    bool operator!=(const Node& second_node) const;
    virtual bool operator==(const Node& second_node) const; // probably need to change Node* to node
    bool isLeaf() const;
    bool isFull() const;
    bool hasSingleSon() const;
    int getNodeKey() const;
    void setNodeKey(int new_key);
    int getNodeHeight() const;
    void setNodeHeight(int new_height);
    int getNodeBF() const;
    void setNodeBF(int new_balance_factor);
    Node* getNodeLeftSon() const; // const?? -  removed the const
    void setNodeLeftSon(Node* new_left_son);
    Node* getNodeRightSon() const; // const?? -  removed the const
    void setNodeRightSon(Node* new_right_son);
    Node* getNodeParent() const; // const?? - removed the const
    void setNodeParent(Node *new_parent);
    void setParentNewSon(Node* new_son);

    virtual int getExtraData() const = 0;
    virtual int getSecondaryExtraData() const = 0;
    virtual int getThirdExtraData() const = 0;
    virtual void updateExtraData(int index) = 0;
    virtual void copyExtraData(Node* other) = 0;
    virtual int getHistogramData(int index) = 0;
    virtual int getHistogramSum() = 0;

};
#endif //WET1_NODE_H
