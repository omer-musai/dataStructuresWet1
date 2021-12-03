#include "GameSystem.hpp"



void GameSystem::addGroup(int groupId)
{
    Group group(groupId);
    groups.addNode(&group);
}

void GameSystem::addPlayer(int playerId, int groupId, int level)
{
    if (groupId <= 0)
    {
        throw InvalidInput("Invalid input provided to addPlayer.");
    }

    addPlayer(playerId, groups.getValuePtr(groupId), level);
}

void GameSystem::addPlayer(int playerId, Group* groupPtr, int level)
{
    if (playerId <= 0 || level < 0)
    {
        throw InvalidInput("Invalid input provided to addPlayer.");
    }

    Player plr(playerId, level, groupPtr);

    Player *p = players.addNode(&plr);

    groupPtr->addPlayer(*p);

    PlayerById pbi(playerId, p);
    playersById.addNode(&pbi);

    //TODO: Note in the .lyx that log(n_group)<log(n) in increaseLevel so we're still fine.
    if(groupPtr->getSize() == 1)
    {
        nonEmptyGroups.addNode(groupPtr);
    }
}

void GameSystem::removePlayer(int playerId)
{
    if (playerId <= 0)
    {
        throw InvalidInput("Invalid input provided to GameSystem::removePlayer.");
    }

    const Player* plr = playersById.getValuePtr(PlayerById(playerId, nullptr))->getPlayerPtr();

    playersById.removeNode(PlayerById(playerId, plr));
    if (plr->getGroupPtr()->getSize() == 1)
    {
        nonEmptyGroups.removeNode(*plr->getGroupPtr()); //O(log(n)) since there are only at most as many non-empty groups as there are players.
    }
    plr->getGroupPtr()->removeNode(*plr);
    players.removeNode(*plr);
}


void GameSystem::replaceGroup(int groupId, int replacementId)
{
	if (replacementId <= 0 || groupId <= 0 || groupId == replacementId) //Check if last is valid.
	{
		throw InvalidInput("Invalid input in replaceGroup.");
	}
	bool nonEmpty; //TODO: Ensure exception thrown when not enough players printed. (Unrelated to this func)

	Group *group = this->groups.getValuePtr(groupId),
		*replacement = this->groups.getValuePtr(replacementId);

	nonEmpty = group->getSize() > 0;

	if (nonEmpty)
    {
	    nonEmptyGroups.removeNode(*group);
    }

	replacement->mergeGroups(*group);
	this->groups.removeNode(*group);
}

int GameSystem::getHighestLevel(int groupId)
{
    if(groupId == 0)
    {
        throw InvalidInput("Invalid input in getHighestLevel.");
    }

    if(groupId < 0)
    {
        if(players.getSize() == 0)
        {
            return -1;
        }
        return players.getHighest().getId();
    }

    Group* group = groups.getValuePtr(groupId);

    if(group->getSize() == 0)
    {
        return -1;
    }
    return group->getHighest().getId();
}

int* GameSystem::getAllPlayersByLevel(int groupId, int* numOfPlayers)
{
    if(groupId == 0)
    {
        throw InvalidInput("Invalid input in getAllPlayersByLevel.");
    }

    if(groupId < 0)
    {
        *numOfPlayers = players.getSize();
        if(*numOfPlayers == 0)
        {
            return nullptr;
        }

        Player* playersArr = AVLTree<Player>::treeToArray(this->players, true);

        return playersToIds(playersArr, *numOfPlayers);
    }

    Group* group = groups.getValuePtr(groupId);

    *numOfPlayers = group->getSize();
    if(*numOfPlayers == 0)
    {
        return nullptr;
    }

    Player* playersArr = AVLTree<Player>::treeToArray(*group->getPlayers());

    return playersToIds(playersArr, *numOfPlayers);
}

int* GameSystem::playersToIds(Player* playersArr, int n)
{
    int* arr = (int*)malloc(sizeof(int) * n);
    if(!arr)
    {
        throw AllocationError();
    }

    for (int i = 0; i < n; i++)
    {
        arr[i] = playersArr[i].getId();
    }
    delete[] playersArr;
    return arr;
}

int* GameSystem::getGroupsHighestLevel(int numOfGroups)
{
    if(numOfGroups < 1)
    {
        throw InvalidInput("Invalid input in getGroupsHighestLevel.");
    }
    if(numOfGroups < this->nonEmptyGroups.getSize())
    {
        throw Failure();
    }

    int* arr = (int*)malloc(sizeof(int) * numOfGroups);
    if(!arr)
    {
        throw AllocationError();
    }

    Group* groupsArr = AVLTree<Group>::treeToArray(this->nonEmptyGroups);
    for(int i = 0; i < numOfGroups; i++)
    {
        arr[i] = groupsArr[i].getHighest().getId();
    }
    delete[] groupsArr;
    return arr;
}

void GameSystem::increaseLevel(int playerId, int levelIncrease)
{
    if (playerId <= 0 || levelIncrease <= 0)
    {
        throw InvalidInput("Invalid input supplied to increaseLevel.");
    }

    const Player* plr = playersById.getValuePtr(PlayerById(playerId, nullptr))->getPlayerPtr(); //O(log(n))
    int originalLevel = plr->getLevel();
    Group* groupPtr = plr->getGroupPtr();

    removePlayer(playerId); //O(log(n))
    addPlayer(playerId, groupPtr, originalLevel + levelIncrease); //O(log(n)) since a group ptr is passed.
}
