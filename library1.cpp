#include "library1.h"
#include "PlayersManager.h"

void* Init()
{
    PlayersManager* DS;
    try
    {
        DS = new PlayersManager();
    }
    catch (std::bad_alloc& bad_alloc)
    {
        return nullptr;
    }
    return DS;
}

StatusType AddGroup(void *DS, int GroupID)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try
    {
        (static_cast<PlayersManager*>(DS))-> addGroup(GroupID);
    }
    catch (InvalidInput& invalid_input)
    {
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& bad_alloc)
    {
        return ALLOCATION_ERROR;
    }
    catch (GroupIDIsAlreadyTaken& group_id_already_taken)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try
    {
        (static_cast<PlayersManager*>(DS))->addPlayer(PlayerID, GroupID, Level);
    }
    catch (InvalidInput& invalid_input)
    {
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& bad_alloc)
    {
        return ALLOCATION_ERROR;
    }
    catch (GroupDoesNotExist& group_does_not_exists)
    {
        return FAILURE;
    }
    catch (PlayerIDIsAlreadyTaken& player_id_already_taken)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemovePlayer(void *DS, int PlayerID)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try
    {
        (static_cast<PlayersManager*>(DS)->removePlayer(PlayerID));
    }
    catch (InvalidInput& invalid_input)
    {
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& bad_alloc)
    {
        return ALLOCATION_ERROR;
    }
    catch (PlayerDoesNotExist& player_does_not_exists)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try
    {
        (static_cast<PlayersManager*>(DS)->replaceGroup(GroupID, ReplacementID));
    }
    catch (InvalidInput& invalid_input)
    {
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& bad_alloc)
    {
        return ALLOCATION_ERROR;
    }
    catch (GroupDoesNotExist& player_does_not_exists)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try
    {
        (static_cast<PlayersManager*>(DS)->increasePlayerLevel(PlayerID, LevelIncrease));
    }
    catch (InvalidInput& invalid_input)
    {
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& bad_alloc)
    {
        return ALLOCATION_ERROR;
    }
    catch (PlayerDoesNotExist& player_does_not_exists)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try
    {
        (static_cast<PlayersManager*>(DS)->getLeader(GroupID, PlayerID));
    }
    catch (InvalidInput& invalid_input)
    {
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& bad_alloc)
    {
        return ALLOCATION_ERROR;
    }
    catch (GroupDoesNotExist& group_does_not_exists)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try
    {
        (static_cast<PlayersManager*>(DS)->
        getAllPlayersByLevel(GroupID, Players, numOfPlayers));
    }
    catch (InvalidInput& invalid_input)
    {
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& bad_alloc)
    {
        return ALLOCATION_ERROR;
    }
    catch (AllocationError& allocation_error)
    {
        return ALLOCATION_ERROR;
    }
    catch (GroupDoesNotExist& group_does_not_exists)
    {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players)
{
    if (DS == nullptr)
    {
        return INVALID_INPUT;
    }
    try
    {
        (static_cast<PlayersManager*>(DS)->getGroupsLeaders(numOfGroups, Players));
    }
    catch (InvalidInput& invalid_input)
    {
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& bad_alloc)
    {
        return ALLOCATION_ERROR;
    }
    catch (AllocationError& allocation_error)
    {
        return ALLOCATION_ERROR;
    }
    catch (TooFewFullGroups& too_few_full_groups)
    {
        return FAILURE;
    }
    return SUCCESS;
}

void Quit(void **DS)
{
    if(DS == nullptr)
    {
        return;
    }
    delete static_cast<PlayersManager*>(*DS);
    *DS = nullptr;
}