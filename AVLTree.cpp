#include "AVLTree.h"
#include <cassert>
#include <cmath>

const int AVLTree::HEIGHT_FACTOR = 1;
const int AVLTree::INCREMENT_FACTOR = 1;
const int AVLTree::BASE = 2;
const int AVLTree::HEIGHT_OF_NULL_SON = -1;
const int AVLTree::NEGATIVE_IMBALANCE_CONST = -2;
const int AVLTree::POSITIVE_IMBALANCE_CONST = 2;
const char AVLTree::LEFT_SUBTREE_SIGN = 'L';
const char AVLTree::RIGHT_SUBTREE_SIGN = 'R';

AVLTree::AVLTree(): root(nullptr), largest_node(nullptr), size(0){
}

Node *AVLTree::getRoot() {
    return this->root;
}

void AVLTree::setRoot(Node *new_root) {
    this->root = new_root;
}

Node *AVLTree::getLargestNode() {
    return this->largest_node;
}

void AVLTree::setLargestNode(Node *new_largest_node) {
    this->largest_node = new_largest_node;
}
/*
Node* AVLTree::find(Node* node_to_find, Node* node_to_compare){ // we return the node we want the node with the same credanitlas but in the tree
    // at first node_to_compare should be the root
    assert(node_to_find != nullptr);
    if (node_to_compare == nullptr)
    {
        return nullptr;
    }
    else if (*node_to_find == *node_to_compare)
    {
        return node_to_compare;
    }
    else if (*node_to_find < *node_to_compare)
    {
        return find(node_to_find, node_to_compare->getNodeLeftSon());
    }
    else
    {
        return find(node_to_find, node_to_compare->getNodeRightSon());
    }
}*/

Node* AVLTree::find(Node* node_to_find){ // we return the node we want the node with the same credanitlas but in the tree
    if(node_to_find == nullptr)
    {
        return nullptr;
    }
    return findAux(node_to_find, root);
}

Node* AVLTree::findAux(Node* node_to_find, Node *node_to_compare) {
    if (node_to_compare == nullptr)
    {
        return nullptr;
    }
    else if (*node_to_find == *node_to_compare)
    {
        return node_to_compare;
    }
    else if (*node_to_find < *node_to_compare)
    {
        return findAux(node_to_find, node_to_compare->getNodeLeftSon());
    }
    else
    {
        return findAux(node_to_find, node_to_compare->getNodeRightSon());
    }
}

/*
Node* AVLTree::find(int key){ // omri version
    // at first node_to_compare should be the root
    if(key <= 0)
        return nullptr;     // if we want to throw an exception we should do it in the manager level
    return findAux(key,root);
}
*/

int AVLTree::getSubtreeHeight(Node* subtree_root)
{
    if (subtree_root == nullptr)
    {
        return HEIGHT_OF_NULL_SON;
    }
    else
    {
        return subtree_root->getNodeHeight();
    }
}

void AVLTree::NodeHeightAndBFUpdate(Node* node_to_update)
{
    assert(node_to_update != nullptr);
    int left_subtree_height = getSubtreeHeight(node_to_update->getNodeLeftSon());
    int right_subtree_height = getSubtreeHeight(node_to_update->getNodeRightSon());
    node_to_update->setNodeHeight(static_cast<int>(fmax(right_subtree_height, left_subtree_height)) + HEIGHT_FACTOR);
    node_to_update->setNodeBF(left_subtree_height - right_subtree_height);
    node_to_update->updateExtraData(node_to_update->getSecondaryExtraData());
}
void AVLTree::updateNodesOnInsertOrRemoveCourse(Node* node_to_update, bool to_remove)
{
    NodeHeightAndBFUpdate(node_to_update);
    int current_balance_factor = node_to_update->getNodeBF();
    if ((current_balance_factor == NEGATIVE_IMBALANCE_CONST) || (current_balance_factor == POSITIVE_IMBALANCE_CONST))
    {
        performRoll(node_to_update);
        if(!to_remove)
            updateRoot();
    }
}

void AVLTree::updateRoot()
{
    if(root != nullptr)
    {
        Node* root_parent = this->root->getNodeParent();
        if (root_parent != nullptr)
        {
            Node* root_grandparent = root_parent->getNodeParent();
            if (root_grandparent != nullptr)
            {
                this->setRoot(root_grandparent);
            }
            else
            {
                this->setRoot(root_parent);
            }
        }
    }

}

Node* AVLTree::enterNodeToTree(Node* node_to_enter, Node* parent, char subtree_sign)
{
    Node* copy_of_node_to_enter = node_to_enter->clone();
    copy_of_node_to_enter->setNodeParent(parent); // left and right son are still null
    if (subtree_sign == LEFT_SUBTREE_SIGN)
    {
        parent->setNodeLeftSon(copy_of_node_to_enter);
    }
    else
    {
        parent->setNodeRightSon(copy_of_node_to_enter);
    }
    return copy_of_node_to_enter;
}

/*void AVLTree::findPlaceToInsert(Node* node_to_enter, Node* node_to_compare)
{
    assert(node_to_enter != nullptr);
    assert(node_to_compare != nullptr);
    Node* possible_son;
    char subtree_sign;
    if (*node_to_enter == *node_to_compare)
    {
        throw KeyIsAlreadyTaken();
    }
    else if (*node_to_enter < *node_to_compare)
    {
        possible_son = node_to_compare->getNodeLeftSon();// maybe an error will occur here
        subtree_sign = LEFT_SUBTREE_SIGN;
    }
    else
    {
        possible_son = node_to_compare->getNodeRightSon();
        subtree_sign = RIGHT_SUBTREE_SIGN;
    }
    if (possible_son == nullptr)
    {
        enterNodeToTree(node_to_enter, node_to_compare, subtree_sign);
    }
    else
    {
        findPlaceToInsert(node_to_enter, node_to_compare->getNodeLeftSon());
        updateNodesOnInsertOrRemoveCourse(node_to_compare);
    }
}*/


Node* AVLTree::findPlaceToInsertOrRemove(Node *node, Node *node_to_compare, bool to_remove) {
    Node* possible_son;
    Node* cloned_node;
    Node* parent_to_cloned_node;
    char subtree_sign;
    if (*node == *node_to_compare)
    {
        if(!to_remove)
            throw KeyIsAlreadyTaken();
        cloned_node = removeNode(node_to_compare);
        parent_to_cloned_node = cloned_node;
        if(cloned_node != nullptr)
        {
            if(cloned_node->getNodeParent() != nullptr)
                parent_to_cloned_node = cloned_node->getNodeParent();
            updateNodesOnInsertOrRemoveCourse(cloned_node,to_remove);
        }
        return parent_to_cloned_node;
    }
    else if (*node < *node_to_compare)
    {
        possible_son = node_to_compare->getNodeLeftSon();// maybe an error will occur here- why?
        subtree_sign = LEFT_SUBTREE_SIGN;
    }
    else
    {
        possible_son = node_to_compare->getNodeRightSon();
        subtree_sign = RIGHT_SUBTREE_SIGN;
    }
    if (possible_son == nullptr)
    {
        if(!to_remove)
            cloned_node = enterNodeToTree(node, node_to_compare, subtree_sign);
        else
            throw NodeDoesNotExist();
    }
    else
    {
        cloned_node = findPlaceToInsertOrRemove(node, possible_son, to_remove);
        if(!to_remove)
            updateNodesOnInsertOrRemoveCourse(possible_son, to_remove);
        else
        {
            parent_to_cloned_node = cloned_node;
            if(cloned_node != nullptr)
            {
                if(cloned_node->getNodeParent() != nullptr)
                    parent_to_cloned_node = cloned_node->getNodeParent();
                updateNodesOnInsertOrRemoveCourse(cloned_node, to_remove);
            }
            cloned_node = parent_to_cloned_node;
        }
    }
    return cloned_node;
}

void AVLTree::insert(Node* node_to_enter)
{
    Node* cloned_node;
    assert(node_to_enter != nullptr);   //probably an exception the manger handles and can be removed
    if (this->root == nullptr)
    {
        this->root = node_to_enter->clone();
        this->largest_node = this->root;
    }
    else
    {
        cloned_node = findPlaceToInsertOrRemove(node_to_enter, root, false);
        updateNodesOnInsertOrRemoveCourse(root, false);
        if (*(this->largest_node) < *cloned_node)
        {
            this->setLargestNode(cloned_node);
        }
    }
    this->size++;
}

void AVLTree::destroyNode(Node* node_to_destroy)
{
    delete node_to_destroy;
}


void AVLTree::postorder(Node* current_node, void (*func)(Node*))
{
    if (current_node == nullptr)
    {
        return;
    }
    postorder(current_node->getNodeLeftSon(), func);
    postorder(current_node->getNodeRightSon(), func);
    func(current_node);
}
/*
void AVLTree::setNewGroupToPlayer(Node* current_node, const std::shared_ptr<Group>& player_group)
{
    if (current_node == nullptr)
    {
        return;
    }
    setNewGroupToPlayer(current_node->getNodeLeftSon(), player_group);
    LevelNode* level_node = dynamic_cast<LevelNode*>(current_node);
    level_node->getPlayerInstance()->setGroup(player_group);
    setNewGroupToPlayer(current_node->getNodeRightSon(), player_group);
}
*/
void AVLTree::fixTreeHeight()
{
    postorder(root, &NodeHeightAndBFUpdate);
}

AVLTree::~AVLTree() {
    postorder(root, &destroyNode);
}

void AVLTree::leftRoll(Node *unbalanced_node)
{
    assert(unbalanced_node != nullptr);
    Node* right_son = unbalanced_node->getNodeRightSon();
    unbalanced_node->setParentNewSon(right_son);
    assert(right_son != nullptr);
    Node* right_left_son = right_son->getNodeLeftSon();
    right_son->setNodeParent(unbalanced_node->getNodeParent());
    right_son->setNodeLeftSon(unbalanced_node);
    unbalanced_node->setNodeParent(right_son);
    unbalanced_node->setNodeRightSon(right_left_son);
    if(right_left_son != nullptr)
    {
        right_left_son->setNodeParent(unbalanced_node);
    }
    NodeHeightAndBFUpdate(unbalanced_node);
    NodeHeightAndBFUpdate(right_son);
    //still need to understand what exactly to return (what shafran said))
}

void AVLTree::rightRoll(Node *unbalanced_node)
{
    assert(unbalanced_node != nullptr);
    Node *left_son = unbalanced_node->getNodeLeftSon();
    unbalanced_node->setParentNewSon(left_son);
    assert(left_son != nullptr);
    Node *left_right_son = left_son->getNodeRightSon();
    left_son->setNodeParent(unbalanced_node->getNodeParent());
    left_son->setNodeRightSon(unbalanced_node);
    unbalanced_node->setNodeParent(left_son);
    unbalanced_node->setNodeLeftSon(left_right_son);
    if(left_right_son != nullptr)
    {
        left_right_son->setNodeParent(unbalanced_node);
    }
    NodeHeightAndBFUpdate(unbalanced_node);
    NodeHeightAndBFUpdate(left_son);
    //still need to understand what exactly to return (what shafran said))
}

void AVLTree::performLL(Node *unbalanced_node)
{
    assert(unbalanced_node != nullptr);
    rightRoll(unbalanced_node);
}

void AVLTree::performLR(Node *unbalanced_node)
{
    assert(unbalanced_node != nullptr);
    assert(unbalanced_node->getNodeLeftSon() != nullptr);
    leftRoll(unbalanced_node->getNodeLeftSon());
    rightRoll(unbalanced_node);
}

void AVLTree::performRR(Node *unbalanced_node)
{
    assert(unbalanced_node != nullptr);
    leftRoll(unbalanced_node);
}

void AVLTree::performRL(Node *unbalanced_node)
{
    assert(unbalanced_node != nullptr);
    assert(unbalanced_node->getNodeRightSon() != nullptr);
    rightRoll(unbalanced_node->getNodeRightSon());
    leftRoll(unbalanced_node);
}

void AVLTree::performRoll(Node *unbalanced_node)
{
    assert(unbalanced_node != nullptr);
    int unbalanced_node_balance_factor = unbalanced_node->getNodeBF();
    if (unbalanced_node_balance_factor == POSITIVE_IMBALANCE_CONST)
    {
        // impossible that it's null but need to check it
        assert(unbalanced_node->getNodeLeftSon() != nullptr);
        if (unbalanced_node->getNodeLeftSon()->getNodeBF() >= 0)
        {
            performLL(unbalanced_node);
        }
        else
        {
            performLR(unbalanced_node);
        }
    }
    else
    {
        assert(unbalanced_node->getNodeRightSon() != nullptr);
        if (unbalanced_node->getNodeRightSon()->getNodeBF() <= 0)
        {
            performRR(unbalanced_node);
        }
        else
        {
            performRL(unbalanced_node);
        }
    }
}

void AVLTree::remove(Node *node_to_remove) {
    assert(node_to_remove != nullptr);
    if(root == nullptr)
    {
        throw NodeDoesNotExist();
    }
    if(*node_to_remove == *(this->largest_node))
    {
        Node* new_largest_node = this->largest_node->getNodeLeftSon();
        if(new_largest_node != nullptr)
        {
            while(new_largest_node->getNodeRightSon() != nullptr)
            {
                new_largest_node = new_largest_node->getNodeRightSon();
            }
            this->setLargestNode(new_largest_node);
        } else if(largest_node->getNodeParent() != nullptr)
        {
            this->setLargestNode(largest_node->getNodeParent());
        } else
        {
            this->setLargestNode(nullptr);
        }
    }

    root = findPlaceToInsertOrRemove(node_to_remove,root,true);
    if(root != nullptr)
    {
        updateRootRemove();
        //updateNodesOnInsertOrRemoveCourse(root);
    }
    this->size--;
}


Node** AVLTree::inOrderCopyFromTreeToArray() {
    if(size == 0)
    {
        return nullptr;
    }
    Node** nodes_array = new Node*[size];
    int index = 0;
    inOrderCopyFromTreeToArrayAux(nodes_array, root, &index);
    return nodes_array;
}

void AVLTree::inOrderCopyFromTreeToArrayAux(Node **current_node_array, Node *current_node_tree, int* i) {
    if(current_node_tree == nullptr)
    {
        return;
    }
    this->inOrderCopyFromTreeToArrayAux(current_node_array,current_node_tree->getNodeLeftSon(),i);
    current_node_array[(*i)] = current_node_tree->clone();
    Node* temp_node = current_node_array[(*i)];
    current_node_array[(*i)]->setNodeParent(nullptr);
    current_node_array[(*i)++]->setNodeLeftSon(nullptr);
    this->inOrderCopyFromTreeToArrayAux(current_node_array, current_node_tree->getNodeRightSon(),i);
    temp_node->setNodeRightSon(nullptr);
}

Node* AVLTree::preOrderCopy(Node *current_node) {
    if(current_node == nullptr)
    {
        return nullptr;
    }
    Node* new_node = current_node->clone();
    Node* left_son = preOrderCopy(current_node->getNodeLeftSon());
    Node* right_son = preOrderCopy(current_node->getNodeRightSon());
    new_node->setNodeLeftSon(left_son);
    new_node->setNodeRightSon(right_son);
    if(left_son != nullptr)
    {
        left_son->setNodeParent(new_node);
    }
    if(right_son != nullptr)
    {
        right_son->setNodeParent(new_node);
    }
    return new_node;
}

AVLTree::AVLTree(const AVLTree &other): root(nullptr), largest_node(nullptr), size(other.size) {
    root = preOrderCopy(other.root);
    largest_node = find(other.largest_node);
}

Node **AVLTree::mergeArrays(Node **array1, Node **array2, int size1, int size2) {
    if(size1+size2 == 0)
    {
        return nullptr;
    }
    Node** merge_array = new Node*[size1+size2];
    int array1_ind = 0, array2_ind = 0;
    bool array1_full = false, array2_full = false;
    for(int i=0; i < size1+size2; i++)
    {
        if(array1_ind == size1)
        {
            array1_full = true;
        }
        else if(array2_ind == size2)
        {
            array2_full = true;
        }
        if((!array1_full) && ((array2_full) || (*array1[array1_ind] < *array2[array2_ind])))
        {
            merge_array[i] = array1[array1_ind]->clone();
            array1_ind++;
        }
        else
        {
            merge_array[i] = array2[array2_ind]->clone();
            array2_ind++;
        }
    }
    return merge_array;
}

void AVLTree::buildEmptyTree(AVLTree* empty_tree, int num_of_nodes) {
    if(num_of_nodes == 0)
    {
        return;
    }
    empty_tree->setSize(num_of_nodes);
    int height = ceil(log2(num_of_nodes + INCREMENT_FACTOR)) - INCREMENT_FACTOR;
    int nodes_to_remove = pow(BASE, height + INCREMENT_FACTOR) - INCREMENT_FACTOR - num_of_nodes;
    int num_of_leaves = pow(BASE, height) - nodes_to_remove;
    int counter = 0;
    empty_tree->root = buildEmptyTreeAux(height, &counter, num_of_leaves);
}

int AVLTree::getSize() const{
    return size;
}

void AVLTree::setSize(int new_size) {
    size = new_size;
}

Node *AVLTree::buildEmptyTreeAux(int height, int *counter, int num_of_leaves) {  //code duplication but hard to avoid
    if(height == 0)
    {
        if(*counter == num_of_leaves)
        {
            return nullptr;
        }
        else
        {
            (*counter)++;
        }

    }

    else if(height == HEIGHT_OF_NULL_SON)
    {
        return nullptr;
    }
    Node* new_node = EmptyNode().clone();
    Node* left_son = buildEmptyTreeAux(height-INCREMENT_FACTOR, counter, num_of_leaves);
    Node* right_son = buildEmptyTreeAux(height-INCREMENT_FACTOR, counter, num_of_leaves);
    new_node->setNodeLeftSon(left_son);
    new_node->setNodeRightSon(right_son);
    NodeHeightAndBFUpdate(new_node);
    if(left_son != nullptr)
    {
        left_son->setNodeParent(new_node);
    }
    if(right_son != nullptr)
    {
        right_son->setNodeParent(new_node);
    }
    return new_node;
}

AVLTree* AVLTree::mergeTrees(AVLTree *tree1, AVLTree *tree2) {    //maybe change to pointers instead of reference
    AVLTree *merged_tree =  new AVLTree();
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
    return merged_tree;
}

void AVLTree::destroyArray(Node **nodes, int size_array) {
    if(nodes == nullptr)
    {
        return;
    }
    for(int i=0; i < size_array; i++)
    {
        delete nodes[i];
    }
    delete[] nodes;
}

Node* AVLTree::inOrderCopyFromArrayToTree(Node* current_node, Node** merged_array, int *i) {
    if(current_node == nullptr)
    {
        return nullptr;
    }
    Node* left_son = inOrderCopyFromArrayToTree(current_node->getNodeLeftSon(),merged_array,i);
    Node* temp_node = current_node;
    current_node = merged_array[(*i)++]->clone();
    Node* right_son = inOrderCopyFromArrayToTree(temp_node->getNodeRightSon(),merged_array,i);
    current_node->setNodeLeftSon(left_son);
    if(left_son != nullptr)
    {
        left_son->setNodeParent(current_node);
    }
    current_node->setNodeRightSon(right_son);
    if(right_son != nullptr)
    {
        right_son->setNodeParent(current_node);
    }
    current_node->setNodeHeight(temp_node->getNodeHeight());
    current_node->setNodeBF(temp_node->getNodeBF());
    delete temp_node;
    return current_node;
}

bool AVLTree::contains(Node *node_to_find) {
    return (find(node_to_find) != nullptr);
}

Node* AVLTree::removeNode(Node *node_to_remove) {
    Node* node_to_return;
    if(node_to_remove->isLeaf())
    {
        if(*node_to_remove == *(this->root))
        {
            delete this->root;
            this->root = nullptr;
            this->largest_node = nullptr;
            return nullptr;
        }
        node_to_return = node_to_remove->getNodeParent();
        node_to_remove->setParentNewSon(nullptr);
    }
    else if(node_to_remove->hasSingleSon())
    {
        Node *son = node_to_remove->getNodeLeftSon();
        Node *parent_to_remove = node_to_remove->getNodeParent();
        if (son == nullptr)
            son = node_to_remove->getNodeRightSon();
        son->setNodeParent(parent_to_remove);
        if (parent_to_remove != nullptr)
        {
            node_to_remove->setParentNewSon(son);
            node_to_return = parent_to_remove;
        } else
            node_to_return = son;
    } else
    {
        //swap nodes
        Node* node_swapped = node_to_remove->getNodeRightSon();
        Node* itr = swapNodeToRemove(node_to_remove, &node_swapped);
        Node* parent_to_itr;
        //fix height and bf
        while(*itr != *node_swapped)
        {
            parent_to_itr = itr->getNodeParent();
            updateNodesOnInsertOrRemoveCourse(itr, true);
            itr = parent_to_itr;
        }
        node_to_return = itr;
    }
    node_to_remove->setNodeRightSon(nullptr);
    node_to_remove->setNodeLeftSon(nullptr);
    delete node_to_remove;
    return node_to_return;
}

Node* AVLTree::swapNodeToRemove(Node *node_to_remove, Node** node_swapped) {
    Node* right_son_of_remove = node_to_remove->getNodeRightSon();
    Node* left_son_of_remove = node_to_remove->getNodeLeftSon();
    Node* parent_to_remove = node_to_remove->getNodeParent();

    Node* smallest_node_bigger_than_remove = right_son_of_remove;
    if(smallest_node_bigger_than_remove->getNodeLeftSon() == nullptr)
        //this means that the right son of remove is the smallest node
    {
        if(parent_to_remove != nullptr)
            node_to_remove->setParentNewSon(right_son_of_remove);

        left_son_of_remove->setNodeParent(right_son_of_remove);
        right_son_of_remove->setNodeLeftSon(left_son_of_remove);

        right_son_of_remove->setNodeParent(parent_to_remove);

        *node_swapped = right_son_of_remove;
        return right_son_of_remove;
    }
    while(smallest_node_bigger_than_remove->getNodeLeftSon() != nullptr)
    {
        smallest_node_bigger_than_remove = smallest_node_bigger_than_remove->getNodeLeftSon();
    }
    *node_swapped = smallest_node_bigger_than_remove;
    Node* parent_to_smallest_node_bigger_than_remove = smallest_node_bigger_than_remove->getNodeParent();
    Node* smallest_node_bigger_than_remove_right_son = smallest_node_bigger_than_remove->getNodeRightSon();
    smallest_node_bigger_than_remove->setParentNewSon(smallest_node_bigger_than_remove_right_son);
    if(smallest_node_bigger_than_remove_right_son != nullptr)
        smallest_node_bigger_than_remove_right_son->setNodeParent(parent_to_smallest_node_bigger_than_remove);

    smallest_node_bigger_than_remove->setNodeRightSon(right_son_of_remove);
    smallest_node_bigger_than_remove->setNodeLeftSon(left_son_of_remove);
    smallest_node_bigger_than_remove->setNodeParent(parent_to_remove);
    right_son_of_remove->setNodeParent(smallest_node_bigger_than_remove);
    left_son_of_remove->setNodeParent(smallest_node_bigger_than_remove);
    if(parent_to_remove != nullptr)
        node_to_remove->setParentNewSon(smallest_node_bigger_than_remove);

    return parent_to_smallest_node_bigger_than_remove;
}

bool AVLTree::isBalanced(Node *node, bool carry){
    if(node == nullptr) return true;

    carry = isBalanced(node->getNodeLeftSon(), carry) && carry;
    carry = isBalanced(node->getNodeRightSon(), carry) && carry;

    //node->calculateHeightAndBalance();

    return carry && abs(node->getNodeBF()) < POSITIVE_IMBALANCE_CONST;
}


bool AVLTree::isBalanced(){
    return isBalanced(this->root, true);
}

void AVLTree::updateRootRemove() {
    while(root->getNodeParent() != nullptr)
        root = root->getNodeParent();
}

void AVLTree::getAllPlayersByLevel(Node* current_node, int* players_arr, int* counter)
{
    if (current_node == nullptr)
    {
        return;
    }
    getAllPlayersByLevel(current_node->getNodeRightSon(), players_arr, counter);
    players_arr[(*counter)++] = current_node->getNodeKey();
    getAllPlayersByLevel(current_node->getNodeLeftSon(), players_arr, counter);
}

/*
void AVLTree::getGroupsLeaders(Node* current_node, int* players_arr, int* counter, int num_of_groups)
{
    if (*counter == num_of_groups)
    {
        return;
    }

    if (current_node == nullptr)
    {
        return;
    }

    getGroupsLeaders(current_node->getNodeLeftSon(), players_arr, counter, num_of_groups);

    if (*counter != num_of_groups)
    {
        GroupNode* group_node_in_full_groups_tree = dynamic_cast<GroupNode*>(current_node); // we do it only if group_node != nullptr and in the counter range
        assert(group_node_in_full_groups_tree->getGroup() == nullptr);
        assert(group_node_in_full_groups_tree->getGroup()->getPlayersTree() == nullptr);
        assert(group_node_in_full_groups_tree->getGroup()->getPlayersTree()->getLargestNode() == nullptr); // there is at least one group
        players_arr[(*counter)++] = group_node_in_full_groups_tree->getGroup()->getPlayersTree()
                ->getLargestNode()->getNodeKey();
    }
    else
    {
        return;
    }
    getGroupsLeaders(current_node->getNodeRightSon(), players_arr, counter, num_of_groups);

}*/



// copy constructor?
/*
Node* AVLTree::find(Node* node_to_find, bool *is_found)
{ // if we want to make it with recursion so we need to change our goal (insert wont use find)
    assert(node_to_find != nullptr);
    if (root == nullptr)
    {
        *is_found = false;
        return root; // we must check if we got null and deal with it properly (enter to the root)
    }
    Node* current_node = root;
    Node* current_son;
    while (true)
    {
        if (*node_to_find == *current_node) {
            *is_found = true;
            return current_node;
        } else if (*node_to_find < *current_node) {
            current_son = current_node->getNodeLeftSon();
        } else {
            current_son = current_node->getNodeRightSon();
        }
        if (isSonSubtree(current_son, is_found)) // copy constructor ??
        {
            current_node = current_son;
        } else {
            return current_node;
        }
    }
}
*/

/*
void AVLTree::insert(Node *p_node_to_enter) {
    assert(p_node_to_enter != nullptr);
    if (this->root == nullptr)
    {
        this->root = p_node_to_enter;
        this->largest_node = p_node_to_enter;
    }
    bool is_node_found = true;
    Node* last_node_in_route = find(p_node_to_enter, &is_node_found);
    if (is_node_found)
    {
        return; // need to decide what to do here maybe throw exception !
    }
    else
    {
        if (*p_node_to_enter < *last_node_in_route)
        {
            last_node_in_route->setNodeLeftSon(p_node_to_enter);
        }
        last_node_in_route->setNodeRightSon(p_node_to_enter);
        //gilgulim!!!!!!
    }
    if (*(this->largest_node) < *p_node_to_enter)
    {
        this->setLargestNode(p_node_to_enter);
    }
}
*/


//        Node* smallest_node_bigger = node_to_remove->getNodeRightSon();
//        while(smallest_node_bigger->getNodeLeftSon() != nullptr)
//        {
//            smallest_node_bigger = smallest_node_bigger->getNodeLeftSon();
//        }
//        Node* right_son_of_node_to_remove = node_to_remove->getNodeRightSon();
//        Node* right_son_of_smallest_node_bigger = smallest_node_bigger->getNodeRightSon();
//        Node* parent_of_smallest_node_bigger = smallest_node_bigger->getNodeParent();
//        smallest_node_bigger->setParentNewSon(right_son_of_smallest_node_bigger);
//        smallest_node_bigger->setNodeParent(node_to_remove->getNodeParent());
//        smallest_node_bigger->setNodeLeftSon(node_to_remove->getNodeLeftSon());
//        node_to_remove->setParentNewSon(smallest_node_bigger);
//        node_to_remove->getNodeLeftSon()->setNodeParent(smallest_node_bigger);
//        if(*smallest_node_bigger == *(right_son_of_node_to_remove))
//        {
//            parent_of_smallest_node_bigger = smallest_node_bigger;
//        } else
//        {
//            smallest_node_bigger->setNodeRightSon(node_to_remove->getNodeRightSon());
//            node_to_remove->getNodeRightSon()->setNodeParent(smallest_node_bigger);
//        }