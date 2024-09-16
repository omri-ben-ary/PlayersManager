#include "EmptyNode.h"

EmptyNode::EmptyNode(): Node(0) {}

Node *EmptyNode::clone() {
    return new EmptyNode(*this);
}

int EmptyNode::getExtraData() const {
    return 0;
}

void EmptyNode::updateExtraData(int index) {

}

int EmptyNode::getSecondaryExtraData() const {
    return 0;
}

void EmptyNode::copyExtraData(Node* other) {

}

int EmptyNode::getHistogramData(int index) {
    return 0;
}

int EmptyNode::getHistogramSum() {
    return 0;
}

int EmptyNode::getThirdExtraData() const {
    return 0;
}

