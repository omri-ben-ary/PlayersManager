//
// Created by DELL on 26/11/2021.
//

#include "Node.h"
#include <cassert>

const int Node::HEIGHT_OF_LEAF = 0;

Node::Node(int key): key(key), height(0), balance_factor(0), left_son(nullptr), right_son(nullptr),  parent(nullptr){
}

bool Node::operator<(const Node& second_node) const
{
    return (this->key < second_node.key);
}

bool Node::operator>(const Node& second_node) const
{
    return (second_node < *this);
}

bool Node::operator==(const Node &second_node) const {
    return (this->key == second_node.key);
}

bool Node::isLeaf() const
{
    return ((this->left_son == nullptr) && (this->right_son == nullptr));
}

bool Node::isFull() const
{
    return (this->left_son != nullptr && this->right_son != nullptr);
}

bool Node::hasSingleSon() const
{
    return (!this->isLeaf()) && (!this->isFull());
}

int Node::getNodeKey() const
{
    return this->key;
}

void Node::setNodeKey(int new_key)
{
    this->key = new_key;
}

int Node::getNodeHeight() const
{
    if (this->isLeaf())
    {
        return HEIGHT_OF_LEAF;
    }
    return this->height;
}

void Node::setNodeHeight(int new_height)
{
    this->height = new_height;
}

int Node::getNodeBF() const
{
    return this->balance_factor;
}

void Node::setNodeBF(int new_balance_factor)
{
    this->balance_factor = new_balance_factor;
}

Node* Node::getNodeLeftSon() const
{
    return this->left_son;
}

void Node::setNodeLeftSon(Node *new_left_son)
{
    this->left_son = new_left_son;
}

Node* Node::getNodeRightSon() const
{
    return this->right_son;
}

void Node::setNodeRightSon(Node *new_right_son)
{
    this->right_son = new_right_son;
}

Node* Node::getNodeParent() const
{
    return this->parent;
}

void Node::setNodeParent(Node *new_parent)
{
    this->parent = new_parent;
}

void Node::setParentNewSon(Node* new_son)
{
    Node *node_parent = this->getNodeParent();
    if (node_parent != nullptr)
    {
        Node* left_son = this->getNodeParent()->getNodeLeftSon();
        if ((left_son != nullptr) && (*this == *(left_son)) )
        {
            node_parent->setNodeLeftSon(new_son);
            return;
        }
        node_parent->setNodeRightSon(new_son);

    }
}

bool Node::operator>=(const Node &second_node) const {
    return !(*this < second_node);
}

bool Node::operator!=(const Node &second_node) const {
    return !(*this == second_node);
}



/*
Node::Node(int key): key(key), height(-1), balance_factor(0), left_son(nullptr), right_son(nullptr),  parent(nullptr){
}

int Node::getNodeKey() const {
    return this->key;
}

int Node::getNodeHeight() const {
    return this->height;
}

void Node::setNodeHeight(const int new_height) {
    this->height = new_height;
}

int Node::getNodeBF() const {
    return this->balance_factor;
}

void Node::setNodeBF(int new_balance_factor) {
    this->balance_factor = new_balance_factor;
}

std::shared_ptr<Node> Node::getNodeLeftSon() const {
    return std::shared_ptr<Node>(this->left_son);
}

void Node::setNodeLeftSon(const std::shared_ptr<Node>& new_left_son) {
    this->left_son = new_left_son;
}

std::shared_ptr<Node> Node::getNodeRightSon() const {
    return std::shared_ptr<Node>(this->right_son);
}

void Node::setNodeRightSon(const std::shared_ptr<Node>& new_right_son) {
    this->right_son = std::move(new_right_son);
}

std::shared_ptr<Node> Node::getNodeParent() const {
    return std::shared_ptr<Node>(this->parent);
}

void Node::setNodeParent(const std::shared_ptr<Node>& new_parent) {
    this->parent = std::move(new_parent);
}
*/


/*
Node::Node(int key): key(key), height(-1), balance_factor(0), left_son(nullptr), right_son(nullptr),  parent(nullptr){
}

int Node::getNodeKey() const {
    return this->key;
}

int Node::getNodeHeight() const {
    return this->height;
}

void Node::setNodeHeight(const int new_height) {
    this->height = new_height;
}

int Node::getNodeBF() const {
    return this->balance_factor;
}

void Node::setNodeBF(int new_balance_factor) {
    this->balance_factor = new_balance_factor;
}

std::shared_ptr<Node> Node::getNodeLeftSon() const {
    return std::shared_ptr<Node>(this->left_son);
}

void Node::setNodeLeftSon(const std::shared_ptr<Node>& new_left_son) {
    this->left_son = new_left_son;
}

std::shared_ptr<Node> Node::getNodeRightSon() const {
    return std::shared_ptr<Node>(this->right_son);
}

void Node::setNodeRightSon(const std::shared_ptr<Node>& new_right_son) {
    this->right_son = std::move(new_right_son);
}

std::shared_ptr<Node> Node::getNodeParent() const {
    return std::shared_ptr<Node>(this->parent);
}

void Node::setNodeParent(const std::shared_ptr<Node>& new_parent) {
    this->parent = std::move(new_parent);
}
*/