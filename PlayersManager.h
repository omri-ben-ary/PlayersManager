#ifndef AVLTREE_CPP_PLAYERSMANAGER_H
#define AVLTREE_CPP_PLAYERSMANAGER_H

#include "AVLTree.h"
#include "GroupNode.h"
#include "PlayerNode.h"
#include "LevelNode.h"
#include "PlayersManagerExceptions.h"

class PlayersManager{
private:
    AVLTree* all_players_tree;
    AVLTree* levels_tree;
    AVLTree* all_groups_tree;
    AVLTree* full_groups_tree;

    static const int TEMP_VALUE;
    static const int NO_PLAYERS_IN_GROUP;
    static const int MIN_NUM_OF_GROUPS;

    static Node* createPlayerNode(int player_id, int player_level, Group* player_group);
    static Node* createGroupNode(int group_id);
    static Node* createLevelNode(int player_id, Player* player);
    static Node* generateTempPlayerNode(int player_id);
    static Node* generateTempLevelNode(int player_id, int player_level);
    void addGroupToFullGroupsTree(GroupNode *new_group_node);

public:
    PlayersManager();
    PlayersManager(const PlayersManager& other_manager) = default;
    PlayersManager& operator=(const PlayersManager& other_manager) = default;
    ~PlayersManager();
    void addGroup(int group_id);
    void addPlayer(int player_id, int group_id, int level);
    void removePlayer(int player_id);
    void replaceGroup(int group_id, int replacement_id);
    void increasePlayerLevel(int player_id, int level_to_increase);
    void getLeader(int group_id, int* leader);
    void getAllPlayersByLevel(int group_id, int** players, int* num_of_players);
    void getGroupsLeaders(int num_of_groups, int** players);

    void setNewGroupToPlayer(Node* current_node, Group* player_group);


    void getGroupsLeadersAux(Node *current_node, int *players_arr, int *counter, int num_of_groups);

    static void destroyNode(Node *node_to_destroy);

    void destroyPlayersInNodes(Node* current_node);

    void destroyGroupsInNodes(Node *current_node);
};
#endif //AVLTREE_CPP_PLAYERSMANAGER_H
