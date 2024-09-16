#ifndef AVLTREE_CPP_TREEEXCEPTIONS_H
#define AVLTREE_CPP_TREEEXCEPTIONS_H

#include <iostream>

class TreeExceptions : public std::exception {
public:
    TreeExceptions() = default;
    ~TreeExceptions() override = default;
};

class IllegalArgument : public TreeExceptions {
public:
    IllegalArgument() =  default;
};

class TreeIsEmpty : public TreeExceptions {
public:
    TreeIsEmpty() = default;
};

class KeyIsAlreadyTaken : public TreeExceptions {
public:
    KeyIsAlreadyTaken() = default;
};

class NodeDoesNotExist : public TreeExceptions {
public:
    NodeDoesNotExist() = default;
};
#endif //AVLTREE_CPP_TREEEXCEPTIONS_H
