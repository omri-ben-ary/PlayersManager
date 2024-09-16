#include "GroupNode.h"

GroupNode::GroupNode(int id): Node(id), group(new Group(id)) {    //check if it is not a memory leak
}

GroupNode::GroupNode(int id, Group *new_group): Node(id), group(new_group) {

}

Node *GroupNode::clone() {
    return new GroupNode(*this);
}

int GroupNode::getExtraData() const {
    return this->group->getPlayersTree()->getSize();
}

Group* GroupNode::getGroup() {
    return this->group;
}

void GroupNode::setGroup(Group* group_to_set) {
    this->group = group_to_set;
}

void GroupNode::updateExtraData(int index) {

}

int GroupNode::getSecondaryExtraData() const {
    return 0;
}

void GroupNode::copyExtraData(Node *other) {

}

int GroupNode::getHistogramData(int index) {
    return 0;
}

int GroupNode::getHistogramSum() {
    return 0;
}

int GroupNode::getThirdExtraData() const {
    return 0;
}

