#ifndef MAIN_CPP_AVLTREE_H
#define MAIN_CPP_AVLTREE_H


#include "EmptyNode.h"
#include "Node.h"
#include "TreeExceptions.h"

class AVLTree
{
private:
    Node* root;
    Node* largest_node;
    int size;

    static const int HEIGHT_FACTOR;
    static const int HEIGHT_OF_NULL_SON;
    static const int NEGATIVE_IMBALANCE_CONST;
    static const int POSITIVE_IMBALANCE_CONST;
    static const char LEFT_SUBTREE_SIGN;
    static const char RIGHT_SUBTREE_SIGN;
    static const int INCREMENT_FACTOR;
    static const int BASE;

    //void findPlaceToInsert(Node* node_to_enter, Node* node_to_compare);
    // used in insert()
    static int getSubtreeHeight(Node* subtree_root);
    static void NodeHeightAndBFUpdate(Node* node);
    void updateNodesOnInsertOrRemoveCourse(Node *node_to_update, bool to_remove);        //used also in remove
    static Node* enterNodeToTree(Node* node_to_enter, Node* parent, char subtree_sign);
    Node* findPlaceToInsertOrRemove(Node* node, Node* node_to_compare, bool to_remove);     //used also in remove
    void updateRoot();

    // used for insert() and remove()
    static void leftRoll(Node* unbalanced_node);
    static void rightRoll(Node* unbalanced_node);
    static void performLL(Node* unbalanced_node);
    static void performLR(Node* unbalanced_node);
    static void performRR(Node* unbalanced_node);
    static void performRL(Node* unbalanced_node);
    static void performRoll(Node* unbalanced_node);

    // used for remove()
    Node* removeNode(Node* node_to_remove);
    Node* swapNodeToRemove(Node* node_to_remove, Node** node_swapped);
    void updateRootRemove();

    //used for find()
    Node* findAux(Node* node_to_find, Node* node_to_compare);


protected:
    //destroys specific node - used as pointer to function in the ~AVLTree()
    static void destroyNode(Node *node_to_destroy);

    //used for mergeTrees()
    void inOrderCopyFromTreeToArrayAux(Node** current_node_array, Node* current_node_tree, int* i);
    static void buildEmptyTree(AVLTree* empty_tree, int num_of_nodes);
    static Node* buildEmptyTreeAux(int height, int *counter, int num_of_leaves);
    static Node** mergeArrays(Node** array1, Node** array2, int size1, int size2);
    static void destroyArray(Node** nodes, int size_array);
    static Node* inOrderCopyFromArrayToTree(Node* current_node, Node** merged_array, int *i);

public:
    AVLTree();
    AVLTree(const AVLTree& other);
    AVLTree& operator=(const AVLTree&) = default;
    virtual ~AVLTree();
    Node* getRoot();
    void setRoot(Node* new_root);       //move to private - not now
    Node* getLargestNode();
    void setLargestNode(Node* new_largest_node);        //move to private - not now
    int getSize() const;
    void setSize(int new_size);         //move to private - not now
    //Node* find(Node *node_to_compare, Node *node_to_find);
    Node* find(Node *node_to_find); //added auxiliary
    bool contains(Node *node_to_find);
    //Node* find(int key);  //omri edited this function
    void insert(Node* node_to_enter);
    void remove(Node *node_to_remove);
    void fixTreeHeight();   //maybe private?
    void postorder(Node* current_node, void (*func)(Node*));    //maybe make it non generic and private because it serves one function? if not should we add the same for inOrder and preOrder?
    Node** inOrderCopyFromTreeToArray();    //maybe move to private
    Node* preOrderCopy(Node* current_node); //maybe move to private
    AVLTree* mergeTrees(AVLTree* tree1, AVLTree* tree2);

    bool isBalanced(Node *node, bool carry);
    bool isBalanced();

    void getAllPlayersByLevel(Node *current_node, int* players_arr, int* counter);

    //void getGroupsLeaders(Node* current_node, int* players_arr, int* counter, int num_of_groups);
    //void setNewGroupToPlayer(Node *current_node, const shared_ptr<Group>& player_group);


};


#endif //MAIN_CPP_AVLTREE_H
