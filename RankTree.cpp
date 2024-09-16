#include "RankTree.h"
#include <cassert>

const int RankTree::INCREMENT = 1;

RankTree::RankTree(): AVLTree(), zero_histogram() {

}

RankTree::RankTree(RankTree &other): AVLTree(other), zero_histogram() {
    for(int i = 0; i < MAX_SCALE; i++)
    {
        zero_histogram[i] = other.zero_histogram[i];
    }
}

RankTree::~RankTree() {
    postorder(this->getRoot(), &destroyNode);
}

int RankTree::scoreRank(Node *node, int score) {
    int rank = 0;
    Node* itr = this->getRoot();
    while(itr != nullptr && *itr != *node)
    {
        if(*itr < *node)
        {
            itr = itr->getNodeLeftSon();
        }
        else
        {
            rank += itr->getNodeLeftSon()->getHistogramData(score) + INCREMENT;
            itr = itr->getNodeRightSon();
        }
    }
    rank += itr->getNodeLeftSon()->getHistogramData(score) + INCREMENT;
    return rank;
}

int RankTree::nodeRank(Node *node) {
    int rank = 0;
    Node* itr = this->getRoot();
    while(itr != nullptr && *itr != *node)
    {
        if(*itr < *node)
        {
            itr = itr->getNodeLeftSon();
        }
        else
        {
            rank += itr->getNodeLeftSon()->getHistogramSum() + INCREMENT;
            itr = itr->getNodeRightSon();
        }
    }
    rank += itr->getNodeLeftSon()->getHistogramSum() + INCREMENT;
    return rank;
}

void RankTree::updateZeroHistogram(int index, bool decrease) {
    if(decrease)
        zero_histogram[index]--;
    else
        zero_histogram[index]++;
}

int RankTree::sumRank(Node *node) {
    int rank = 0;
    Node* itr = this->getRoot();
    while(itr != nullptr && *itr != *node)
    {
        if(*itr < *node)
        {
            itr = itr->getNodeLeftSon();
        }
        else
        {
            rank += itr->getNodeLeftSon()->getThirdExtraData() + itr->getExtraData();
            itr = itr->getNodeRightSon();
        }
    }
    rank += itr->getNodeLeftSon()->getHistogramSum() + itr->getExtraData();
    return rank;
}

Node *RankTree::select(int index) {
    return this->selectAux(this->getRoot(), index);
}

Node *RankTree::selectAux(Node *itr, int index) {
    if(itr == nullptr || itr->getNodeLeftSon()->getHistogramSum() == index - INCREMENT)
    {
        return itr;
    }
    if(itr->getNodeLeftSon()->getHistogramSum() > index - INCREMENT)
    {
        return selectAux(itr->getNodeLeftSon(), index);
    }
    return selectAux(itr->getNodeRightSon(), index - itr->getNodeLeftSon()->getHistogramSum() - INCREMENT);
}

RankTree *RankTree::mergeRankTrees(RankTree *tree1, RankTree *tree2) {
    RankTree *merged_tree =  new RankTree();
    Node** array1 = tree1->inOrderCopyFromTreeToArray();
    Node** array2 = tree2->inOrderCopyFromTreeToArray();
    Node** merged_array = mergeArrays(array1,array2,tree1->getSize(),tree2->getSize());
    destroyArray(array1, tree1->getSize());
    destroyArray(array2, tree2->getSize());
    buildEmptyTree(merged_tree, tree1->getSize()+tree2->getSize());
    int index = 0;
    if(merged_array != nullptr)
    {
        Node* new_root = inOrderCopyFromArrayToTree(merged_tree->getRoot(),merged_array,&index);
        merged_tree->setRoot(new_root);
        Node *new_largest_node = merged_tree->getRoot();
        while(new_largest_node->getNodeRightSon() != nullptr)
        {
            new_largest_node = new_largest_node->getNodeRightSon();
        }
        merged_tree->setLargestNode(new_largest_node);
    }

    destroyArray(merged_array, tree1->getSize()+tree2->getSize());
    for(int i = 0; i < MAX_SCALE; i++)
    {
        (merged_tree->zero_histogram)[i] = (tree1->zero_histogram)[i] + (tree2->zero_histogram)[i];
    }
    return merged_tree;
}


