#include "PlayersManager.h"

#include <cassert>

const int PlayersManager::TEMP_VALUE = 1;
const int PlayersManager::MIN_NUM_OF_GROUPS = 1;
const int PlayersManager::NO_PLAYERS_IN_GROUP = -1;

// complexity: O(1)
PlayersManager::PlayersManager(): all_players_tree(new AVLTree()), levels_tree(new AVLTree()), all_groups_tree(new AVLTree()),
                                  full_groups_tree(new AVLTree())
{
}

// complexity: O(1)
PlayersManager::~PlayersManager() {
    delete levels_tree;
    destroyPlayersInNodes(all_players_tree->getRoot());
    delete all_players_tree;
    delete full_groups_tree;
    destroyGroupsInNodes(all_groups_tree->getRoot());
    delete all_groups_tree;
}

Node* PlayersManager::createPlayerNode(int player_id, int player_level, Group* player_group)  // O(1)
{
    return new PlayerNode(player_id, player_level, player_group);
}

Node* PlayersManager::createGroupNode(int group_id) // O(1)
{
    return new GroupNode(group_id);
}

Node* PlayersManager::createLevelNode(int player_id, Player* player) // O(1)
{
    return new LevelNode(player_id, player);
}

void PlayersManager::destroyNode(Node* node_to_destroy)
{
    delete node_to_destroy;
}

void PlayersManager::addGroup(int group_id) // it's actually add group to the all_groups_tree O(log(k))
{
    // step 1 - input checks - complexity O(1)
    if (group_id <= 0)
    {
        throw InvalidInput();
    }

    // step 2 - create new GroupNode- complexity O(1)
    Node* new_group_node = createGroupNode(group_id); // we need to create group with the same id because find/contains can only work with nodes.

    // step 3 - add it to the all_groups_tree - complexity: log(k)
    try
    {
        all_groups_tree->insert(new_group_node);
    }
    catch (KeyIsAlreadyTaken &e) // we won't get here if everything went smooth.
    {
        GroupNode* group_node = dynamic_cast<GroupNode*>(new_group_node);
        Group* group = group_node->getGroup();
        delete new_group_node;
        delete group->getPlayersTree();
        delete group;
        throw GroupIDIsAlreadyTaken(); // we assume that an error will only bק thrown if this id is already exists in insert function
        //  are we ok with the default destructor of groupNode?
    }
    delete new_group_node;
}

void PlayersManager::addGroupToFullGroupsTree(GroupNode* new_group_node) // O(log(num_of_full_groups) <= O(log(k))
{
    try
    {
        full_groups_tree->insert(new_group_node);
    }
    catch (KeyIsAlreadyTaken &e) // if we are here, so we have a huge bug because we only add it if it wasn't there before
    {
        delete new_group_node;
        throw GroupIDIsAlreadyTaken();
    }
}

void PlayersManager::addPlayer(int player_id, int group_id, int player_level) // complexity: O(2*log(k) + 3*log(n) + 1) = O(log(k) + log(n))
{
    // step 1 - input checks - complexity O(1)
    if ((group_id <= 0) || (player_id <= 0) || (player_level < 0))
    {
        throw InvalidInput();
    }

    // step 2 - find the GroupNode with the given id - complexity O(log(k))
    //Node* new_group_node = createGroupNode(group_id); // we want to check if there is already a group with this ID.
    Node* new_group_node = new GroupNode(group_id, nullptr);
    Node* node_in_all_groups_tree = all_groups_tree->find(new_group_node);
    delete new_group_node;
    if (node_in_all_groups_tree == nullptr)
    {
        throw GroupDoesNotExist();
    }
    GroupNode* group_node_in_all_groups_tree = dynamic_cast<GroupNode*>(node_in_all_groups_tree);
    assert(group_node_in_all_groups_tree->getGroup() != nullptr); // CAN'T SEE HOW player_node->getPlayerInstance()==nullptr in this case
    AVLTree* group_players_tree = group_node_in_all_groups_tree->getGroup()->getPlayersTree(); // will use it later
    assert(group_players_tree != nullptr);

    // step 3 - create playerNode to insert later - complexity O(1)
    Group* ptr_to_player_group = group_node_in_all_groups_tree->getGroup(); // check the syntax here maybe something else before it's a shared ptr
    Node* new_player_node = createPlayerNode(player_id, player_level, ptr_to_player_group);
    PlayerNode* player_node_in_all_players_tree = dynamic_cast<PlayerNode*>(new_player_node);

    // step 4 - check that there is not any player with this id, if there is not - lets add him - complexity: log(n)
    try
    {
        all_players_tree->insert(new_player_node); // must succeed (except of bad alloc situation)
    }
    catch (KeyIsAlreadyTaken &error)
    {
        Player* player_to_remove = player_node_in_all_players_tree->getPlayerInstance();
        delete new_player_node;
        delete player_to_remove;
        throw PlayerIDIsAlreadyTaken();
    }

    // step 5 - create LevelNode and add it to the level tree - complexity: log(n)
    assert(player_node_in_all_players_tree != nullptr);
    Node* new_level_node = createLevelNode(player_id,
                                           player_node_in_all_players_tree->getPlayerInstance());
    delete new_player_node;
    levels_tree->insert(new_level_node);

    // step 6 - add the LevelNode to the group players tree - complexity: log(n)
    group_players_tree->insert(new_level_node); // must succeed (except of bad alloc situation)
    delete new_level_node;

    // step 7 - if our player is the first player in the group, add the empty group to full_group_tree - complexity: log(k)
    if (group_players_tree->getSize() == 1) // need to add this group to the full_groups_tree because it's only member is the new_player_node, and therefore it was empty until now.
    {
        GroupNode* group_node = new GroupNode(group_id, group_node_in_all_groups_tree->getGroup());
        addGroupToFullGroupsTree(group_node); // complexity: log(k)
        delete group_node;
        //Node* node = createGroupNode(group_id);
        //GroupNode* group_node = dynamic_cast<GroupNode*>(node); // might be an issue here because we soon add a group node to a tree of Node*
        //group_node->setGroup(group_node_in_all_groups_tree->getGroup()); // don't sure that this is the syntax
        //addGroupToFullGroupsTree(group_node); // complexity: log(k)
        //delete node; // maybe should delete it too
    }
}

Node* PlayersManager::generateTempPlayerNode(int player_id)  // O(1)
{
    Group* ptr_to_player_group (new Group());
    return createPlayerNode(player_id, TEMP_VALUE, ptr_to_player_group);
}

Node* PlayersManager::generateTempLevelNode(int player_id, int player_level)  // O(1)
{
    Player* ptr_to_player (new Player(player_level));
    return createLevelNode(player_id, ptr_to_player);
}

void PlayersManager::removePlayer(int player_id)  // complexity: O(5*log(n)+1) = O(log(n))
{
    // step 1: input check - complexity: O(1)
    if (player_id <= 0)
    {
        throw InvalidInput();
    }

    // step 2: create a temp PlayerNode - complexity: O(1)
    //Node* temp_player_node = generateTempPlayerNode(player_id);
    PlayerNode* temp_player_node = new PlayerNode(player_id);

    // step 3: find the node in the all_players_tree - complexity: O(log(n))
    Node* player_node_in_all_players_tree = all_players_tree->find(temp_player_node);
    delete temp_player_node;
    if (player_node_in_all_players_tree == nullptr)
    {
        throw PlayerDoesNotExist();
    }

    // step 4: create temp LevelNode - complexity O(1)
    int current_player_level = player_node_in_all_players_tree->getExtraData();
    Node* temp_level_node = generateTempLevelNode(player_id, current_player_level);

    //step 5: remove the player from the levels_tree - complexity: O(log(n))
    levels_tree->remove(temp_level_node);

    // step 6: remove the temp LevelNode from the right group_players_tree - complexity O(log(n))
    PlayerNode* player_node = dynamic_cast<PlayerNode*>(player_node_in_all_players_tree);
    assert(player_node->getPlayerInstance() != nullptr); // CAN'T SEE HOW player_node->getPlayerInstance()==nullptr in this case
    Group* player_group = player_node->getPlayerInstance()->getGroup();
    assert(player_group != nullptr);
    assert(player_group->getPlayersTree() != nullptr);
    AVLTree* group_players_tree = player_group->getPlayersTree();
    group_players_tree->remove(temp_level_node);
    delete (dynamic_cast<LevelNode*>(temp_level_node)->getPlayerInstance());
    delete temp_level_node;

    // step 7: check if the level node was the only node his group and if it was we need to remove the node from the full_groups_tree
    // complexity O(log(n)) - because the number of full groups is bounded by the number of players
    if (group_players_tree->getSize() == 0)
    {
        int group_id = player_group->getId();
        Node* group_node = new GroupNode(group_id, nullptr);
        full_groups_tree->remove(group_node);
        delete group_node;
       /* int group_id = player_group->getId();
        Node* group_node = createGroupNode(group_id);
        full_groups_tree->remove(group_node);
        delete group_node;*/
    }

    // step 8: remove the player from the all_players_tree - complexity: O(log(n))
    Player* player_instance_to_delete = player_node->getPlayerInstance();
    all_players_tree->remove(player_node_in_all_players_tree);
    delete player_instance_to_delete;
}

void PlayersManager::setNewGroupToPlayer(Node* current_node, Group* player_group)
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

void PlayersManager::replaceGroup(int group_id, int replacement_id)
{
    // step 1: input check - complexity: O(1)
    if ((group_id <= 0) || (replacement_id <= 0) || (group_id == replacement_id))
    {
        throw InvalidInput();
    }

    // step 2: find the group node with group_id in the all_groups_tree - complexity: O(log(k))
    //Node* group_node_with_group_id = createGroupNode(group_id);
    Node* group_node_with_group_id = new GroupNode(group_id, nullptr);
    Node* group_node_with_group_id_in_all_groups_tree = all_groups_tree->find(group_node_with_group_id);
    delete group_node_with_group_id;
    if (group_node_with_group_id_in_all_groups_tree == nullptr)
    {
        throw GroupDoesNotExist();
    }
    GroupNode* casted_group_node_in_all_groups_tree =
            dynamic_cast<GroupNode*>(group_node_with_group_id_in_all_groups_tree);
    AVLTree* group_players_tree_of_group_id = casted_group_node_in_all_groups_tree->getGroup()->getPlayersTree();

    // step 3: find the group node with replacement_id in the all_groups_tree - complexity: O(log(k))
    //Node* group_node_with_replacement_id = createGroupNode(replacement_id);
    Node* group_node_with_replacement_id = new GroupNode(replacement_id, nullptr);
    Node* group_node_with_replacement_id_in_all_groups_tree = all_groups_tree->find(group_node_with_replacement_id);
    delete group_node_with_replacement_id;
    if (group_node_with_replacement_id_in_all_groups_tree == nullptr)
    {
        throw GroupDoesNotExist();
    }
    if(group_players_tree_of_group_id->getSize() == 0)
    {
        Group* group_to_remove = casted_group_node_in_all_groups_tree->getGroup();
        all_groups_tree->remove(group_node_with_group_id_in_all_groups_tree);
        delete group_to_remove->getPlayersTree();
        delete group_to_remove;
        return;
    }

    GroupNode* casted_replacement_node_in_all_groups_tree =
            dynamic_cast<GroupNode*>(group_node_with_replacement_id_in_all_groups_tree);
    Group* group_of_players = casted_replacement_node_in_all_groups_tree->getGroup();
    AVLTree* group_players_tree_of_replacement_id = group_of_players->getPlayersTree();
    int initial_size_of_group_players_tree_of_replacement_id = group_players_tree_of_replacement_id->getSize();

    // step 4: update the group of the players if the group_players_tree with the group_id - complexity O(n_group)
    setNewGroupToPlayer(group_players_tree_of_group_id->getRoot(), group_of_players);

    // step 5: merge the two trees and enter the result into the merged_tree - complexity O(n_group+ n_replacement)
    AVLTree* merged_tree = all_groups_tree->mergeTrees(group_players_tree_of_group_id, group_players_tree_of_replacement_id);
    group_of_players->setPlayersTree(merged_tree);

    // step 6: remove the GroupNode with group_id from all_players_tree
    AVLTree *tree_of_empty_group = casted_group_node_in_all_groups_tree->getGroup()->getPlayersTree();

    if (tree_of_empty_group->getSize() > 0)
    {
        full_groups_tree->remove(casted_group_node_in_all_groups_tree);
        tree_of_empty_group->postorder(tree_of_empty_group->getRoot(), destroyNode);
        tree_of_empty_group->setSize(0);
        tree_of_empty_group->setRoot(nullptr);
        tree_of_empty_group->setLargestNode(nullptr);
    }
    if (initial_size_of_group_players_tree_of_replacement_id == 0)
    {
        GroupNode* group_node = new GroupNode(replacement_id, group_of_players);
        addGroupToFullGroupsTree(group_node); // complexity: log(k)
        delete group_node;
        /*Node* node = createGroupNode(replacement_id);
        GroupNode* group_node = dynamic_cast<GroupNode*>(node); // might be an issue here because we soon add a group node to a tree of Node*
        group_node->setGroup(group_of_players); // don't sure that this is the syntax
        addGroupToFullGroupsTree(group_node); // complexity: log(k)
        delete node; // maybe should delete it too*/
    }
    //all_groups_tree->remove(group_node_with_group_id_in_all_groups_tree);

    Group* group_to_remove = casted_group_node_in_all_groups_tree->getGroup();
    all_groups_tree->remove(group_node_with_group_id_in_all_groups_tree);
    delete group_to_remove->getPlayersTree();
    delete group_to_remove;
    // need to think about all the other trees and how it affects them
}

void PlayersManager::increasePlayerLevel(int player_id, int level_to_increase) // complexity: O(5*log(n)+1) = O(log(n))
{
    // step 1: input check - complexity: O(1)
    if ((player_id <= 0) || (level_to_increase <= 0))
    {
        throw InvalidInput();
    }
    // step 2: create a temp PlayerNode - complexity: O(1)
    //Node* temp_player_node = generateTempPlayerNode(player_id);
    PlayerNode* temp_player_node = new PlayerNode(player_id);

    // step 3: find the node in the all_players_tree - complexity: O(log(n))
    Node* player_node_in_all_players_tree = all_players_tree->find(temp_player_node);
    delete temp_player_node;
    if (player_node_in_all_players_tree == nullptr)
    {
        throw PlayerDoesNotExist();
    }

    // step 4: create temp LevelNode - complexity O(1)
    int current_player_level = player_node_in_all_players_tree->getExtraData();
    Node* temp_level_node = generateTempLevelNode(player_id, current_player_level);

    //step 5: remove the player from the levels_tree - complexity: O(log(n))
    levels_tree->remove(temp_level_node);

    // step 6: remove the temp LevelNode from the right group_players_tree - complexity O(log(n))
    PlayerNode* player_node = dynamic_cast<PlayerNode*>(player_node_in_all_players_tree);
    assert(player_node->getPlayerInstance() != nullptr); // CAN'T SEE HOW player_node->getPlayerInstance()==nullptr in this case
    Group* player_group = player_node->getPlayerInstance()->getGroup();
    assert(player_group != nullptr);
    assert(player_group->getPlayersTree() != nullptr);
    AVLTree* group_players_tree = player_group->getPlayersTree();
    group_players_tree->remove(temp_level_node);
    delete (dynamic_cast<LevelNode*>(temp_level_node)->getPlayerInstance());
    delete temp_level_node;


    // step 2: create a temp PlayerNode - complexity: O(1)
    // Node* temp_player_node = generateTempPlayerNode(player_id);

    // step 3: find the node in the all_players_tree - complexity: O(log(n))
   /* Node* player_node_in_all_players_tree = all_players_tree->find(temp_player_node);
    delete temp_player_node;
    if (player_node_in_all_players_tree == nullptr)
    {
        throw PlayerDoesNotExist();
    }
    int current_player_level = player_node_in_all_players_tree->getExtraData();
*/
    // step 4: create temp LevelNode - complexity O(1)
    //Node* temp_level_node = generateTempLevelNode(player_id, current_player_level);

    // step 5: remove the node from the levels_tree - complexity O(log(n))
    //levels_tree->remove(temp_level_node); // no need to use the try catch here, although we can for safety reasons.

    // step 6: remove the temp LevelNode from the right group_players_tree - complexity O(log(n))
   /* PlayerNode* player_node = dynamic_cast<PlayerNode*>(player_node_in_all_players_tree);
    assert(player_node->getPlayerInstance() != nullptr); // CAN'T SEE HOW player_node->getPlayerInstance()==nullptr in this case
    std::shared_ptr<Group> player_group = player_node->getPlayerInstance()->getGroup();
    assert(player_group != nullptr);
    assert(player_group->getPlayersTree() != nullptr);
    player_group->getPlayersTree()->remove(temp_level_node);
    delete temp_level_node;*/

    // step 7: create LevelNode to add to trees - complexity O(1)
    player_node->getPlayerInstance()->setLevel(current_player_level + level_to_increase);
    Node* level_node = createLevelNode(player_id, player_node->getPlayerInstance());

    // step 8: insert the LevelNode to the levels_tree - complexity O(log(n))
    levels_tree->insert(level_node);

    // step 9: insert the LevelNode to the group_players_tree - complexity O(log(n))
    player_group->getPlayersTree()->insert(level_node);
    delete level_node;
}

void PlayersManager::getLeader(int group_id, int *leader_id)
{
    // step 1: input check - complexity: O(1)
    if ((leader_id == nullptr ) || (group_id == 0))
    {
        throw InvalidInput();
    }

    // step 2: taking care of the scenario when group_id < 0 - complexity: O(1)
    if (group_id < 0)
    {
        if (levels_tree->getSize() == 0)
        {
            *leader_id = NO_PLAYERS_IN_GROUP;
        }
        else
        {
            *leader_id = levels_tree->getLargestNode()->getNodeKey();
        }
    }

        // step 3: taking care of the scenario when group_id > 0 - complexity: O(log(k))
    else
    {
        // step 3a: find the group node in the all_groups_tree - complexity: O(log(k))
        //Node* group_node = createGroupNode(group_id);
        Node* group_node = new GroupNode(group_id, nullptr);
        GroupNode* group_node_in_all_groups_tree = dynamic_cast<GroupNode*>(all_groups_tree->find(group_node));
        delete group_node;
        if (group_node_in_all_groups_tree == nullptr)
        {
            throw GroupDoesNotExist();
        }

        // step 3b: set the leader - complexity: O(1)
        AVLTree* group_players_tree = group_node_in_all_groups_tree->getGroup()->getPlayersTree();
        if ((group_players_tree == nullptr) || (group_players_tree->getSize() == 0))
        {
            *leader_id = NO_PLAYERS_IN_GROUP;
        }
        else
        {
            *leader_id = group_players_tree->getLargestNode()->getNodeKey();
        }
    }
}

void PlayersManager::getAllPlayersByLevel(int group_id, int **players, int *num_of_players)
{
    // step 1: input check - complexity: O(1)
    if ((players == nullptr) || (num_of_players == nullptr) || (group_id == 0))
    {
        throw InvalidInput();
    }

    // step 2: taking care of the scenario when group_id < 0 - complexity: O(n)
    if (group_id < 0)
    {
        // step 2a: taking care of the scenario when there aren't any players in the structure - complexity: O(1)
        if (levels_tree->getSize() == 0)
        {
            *num_of_players = 0;
            *players = nullptr;
        }

            // step 2b: sort all the players in the structure - complexity: O(n)
        else
        {
            *num_of_players = levels_tree->getSize();
            *players = static_cast<int*>(malloc(sizeof(**players) * (*num_of_players)));
            if (*players == nullptr)
            {
                throw AllocationError();
            }
            int counter = 0;
            levels_tree->getAllPlayersByLevel(levels_tree->getRoot(), *players, &counter); // reverse_inorder and add enter the player_id to the correct place
        }
    }
        // step 3: taking care of the scenario when group_id > 0 - complexity: O(n_group_id + log(k))
    else
    {
        // step 3a: find the group node in the all_groups_tree - complexity: O(log(k))
        Node* group_node = new GroupNode(group_id, nullptr);
        GroupNode* group_node_in_all_groups_tree = dynamic_cast<GroupNode*>(all_groups_tree->find(group_node));
        delete group_node;
        if (group_node_in_all_groups_tree == nullptr)
        {
            throw GroupDoesNotExist();
        }

        // step 3b1: taking care of the scenario when the wanted group is exists but empty - complexity: O(1)
        AVLTree* group_players_tree = group_node_in_all_groups_tree->getGroup()->getPlayersTree();
        *num_of_players = group_players_tree->getSize();
        if ((*num_of_players) == 0)
        {
            *players = nullptr;
        }

        // step 3b2: sort all the players in the group - complexity: O(n_group_id)
        else
        {
            *players = (int*)malloc(sizeof(**players) * (*num_of_players));
            if (*players == nullptr)
            {
                throw AllocationError();
            }
            int counter = 0;
            group_players_tree->getAllPlayersByLevel(group_players_tree->getRoot(), *players, &counter); // reverse_inorder and add enter the player_id to the correct place
        }
    }
}


void PlayersManager::getGroupsLeadersAux(Node* current_node, int* players_arr, int* counter, int num_of_groups)
{
    if (*counter == num_of_groups)
    {
        return;
    }

    if (current_node == nullptr)
    {
        return;
    }

    getGroupsLeadersAux(current_node->getNodeLeftSon(), players_arr, counter, num_of_groups);

    if (*counter != num_of_groups)
    {
        GroupNode* group_node_in_full_groups_tree = dynamic_cast<GroupNode*>(current_node); // we do it only if group_node != nullptr and in the counter range
        assert(group_node_in_full_groups_tree->getGroup() != nullptr);
        assert(group_node_in_full_groups_tree->getGroup()->getPlayersTree() != nullptr);
        assert(group_node_in_full_groups_tree->getGroup()->getPlayersTree()->getLargestNode() != nullptr); // there is at least one group
        players_arr[(*counter)++] = group_node_in_full_groups_tree->getGroup()->getPlayersTree()
                ->getLargestNode()->getNodeKey();
    }
    else
    {
        return;
    }
    getGroupsLeadersAux(current_node->getNodeRightSon(), players_arr, counter, num_of_groups);
}

void PlayersManager::getGroupsLeaders(int num_of_groups, int **players) //complexity: O(num_of_groups + log(k)
{
    // step 1: input check - complexity: O(1)
    if ((players == nullptr) || (num_of_groups < MIN_NUM_OF_GROUPS)) // need to check the players
    {
        throw InvalidInput();
    }

    // step 2: check if the number of full groups to find their leader is bigger than the num of full groups - complexity: O(1)
    if (full_groups_tree->getSize() < num_of_groups)
    {
        throw TooFewFullGroups();
    }

    // step 3: sort the leaders array of the full groups - complexity: O(num_of_groups + log(k))
    *players = static_cast<int*>(malloc(sizeof(**players) * (num_of_groups)));
    if (*players == nullptr)
    {
        throw AllocationError();
    }
    int counter = 0 ;
    getGroupsLeadersAux(full_groups_tree->getRoot(), *players, &counter, num_of_groups);
}

void PlayersManager::destroyPlayersInNodes(Node *current_node) {
    if(current_node == nullptr)
    {
        return;
    }
    destroyPlayersInNodes(current_node->getNodeLeftSon());
    PlayerNode* player_node = dynamic_cast<PlayerNode*>(current_node);
    delete player_node->getPlayerInstance();
    destroyPlayersInNodes(current_node->getNodeRightSon());
}

void PlayersManager::destroyGroupsInNodes(Node *current_node) {
    if(current_node == nullptr)
    {
        return;
    }
    destroyGroupsInNodes(current_node->getNodeLeftSon());
    GroupNode* group_node = dynamic_cast<GroupNode*>(current_node);
    Group* group = group_node->getGroup();
    delete group->getPlayersTree();
    delete group;
    destroyGroupsInNodes(current_node->getNodeRightSon());
}









