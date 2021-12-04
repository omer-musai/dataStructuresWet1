#include "library1.h"
#include "GameSystem.hpp"
#include "game_exceptions.hpp"

#define TRY_CATCH_WRAP(action) \
if (DS == NULL)                \
{                              \
    return INVALID_INPUT;      \
}                              \
try {                          \
    action                     \
}                              \
catch(Failure& exc)            \
{                              \
    return FAILURE;            \
}                              \
catch(std::bad_alloc& exc)     \
{                              \
    return ALLOCATION_ERROR;   \
}                              \
catch(InvalidInput& exc)       \
{                              \
    return INVALID_INPUT;      \
}                              \
return SUCCESS

//TODO:
//* Ensure order of exceptions.
//* Ensure every time FAILURE is thrown, the data structure returns to its previous state.

void *Init()
{
    try
    {
        GameSystem* DS = new GameSystem();
        return (void*)DS;
    }
    catch (std::bad_alloc& exc)
    {
        return NULL;
    }
}

StatusType AddGroup(void *DS, int GroupID)
{
    TRY_CATCH_WRAP(
            ((GameSystem*)DS)->addGroup(GroupID);
    );
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int Level)
{
    TRY_CATCH_WRAP(
            ((GameSystem*)DS)->addPlayer(PlayerID, GroupID, Level);
    );
}

StatusType RemovePlayer(void *DS, int PlayerID)
{
    TRY_CATCH_WRAP(
            ((GameSystem*)DS)->removePlayer(PlayerID);
    );
}

StatusType ReplaceGroup(void *DS, int GroupID, int ReplacementID)
{
    TRY_CATCH_WRAP(
            ((GameSystem*)DS)->replaceGroup(GroupID, ReplacementID);
    );
}

StatusType IncreaseLevel(void *DS, int PlayerID, int LevelIncrease)
{
    TRY_CATCH_WRAP(
            ((GameSystem*)DS)->increaseLevel(PlayerID, LevelIncrease);
    );
}

StatusType GetHighestLevel(void *DS, int GroupID, int *PlayerID)
{
    TRY_CATCH_WRAP(
            *PlayerID = ((GameSystem*)DS)->getHighestLevel(GroupID);
    );
}

StatusType GetAllPlayersByLevel(void *DS, int GroupID, int **Players, int *numOfPlayers)
{
    TRY_CATCH_WRAP(
            *Players = ((GameSystem*)DS)->getAllPlayersByLevel(GroupID, numOfPlayers);
    );
}

StatusType GetGroupsHighestLevel(void *DS, int numOfGroups, int **Players)
{
    TRY_CATCH_WRAP(
            *Players = ((GameSystem*)DS)->getGroupsHighestLevel(numOfGroups);
    );
}

void Quit(void** DS)
{
    delete ((GameSystem*)*DS);
    *DS = NULL;
}
