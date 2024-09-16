#ifndef WET2_RANKTREE_H
#define WET2_RANKTREE_H
#include "AVLTree.h"
class RankTree: public AVLTree {
private:
    static const int MAX_SCALE = 201;
    static const int INCREMENT;
    int zero_histogram[MAX_SCALE];

    Node* selectAux(Node* itr, int index);

public:
    RankTree(); //need to check that array is intiallized to 0's
    RankTree(RankTree& other);
    RankTree& operator=(const RankTree&) = default;
    ~RankTree() override;
    int scoreRank(Node* node, int score);   //does not work when entering a node that does not exist
    int nodeRank(Node* node);   //does not work when entering a node that does not exist
    int sumRank(Node* node);    //does not work when entering a node that does not exist
    Node* select(int index);
    void updateZeroHistogram(int index, bool decrease);
    RankTree* mergeRankTrees(RankTree* tree1, RankTree* tree2);
};
#endif //WET2_RANKTREE_H
