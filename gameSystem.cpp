#include "gameSystem.hpp"



void GameSystem::addGroup(int id)
{
    Group group(id);
    groups.addNode(&group);
}

void GameSystem::addPlayer(int player_id, int group_id, int level)
{
    Group* group_ptr = groups.getValuePtr(group_id);

    Player plr(player_id, level, group_ptr);
   
    Player *p = players.addNode(&plr),
            *p_group = group_ptr->addPlayer(plr);

    PlayerById pbi(player_id, p);
    playersById.addNode(&pbi);

    Group* nonEmptyGroup;

    //TODO: Note in the .lyx that log(n_group)<log(n) in increaseLevel so we're still fine.
    if(group_ptr->getSize() == 1)
    {
        nonEmptyGroup = nonEmptyGroups.addNode(group_ptr);
    }
    else
    {
        nonEmptyGroup = nonEmptyGroups.getValuePtr(group_id);
    }

    p->setNonEmptyGroupPtr(nonEmptyGroup);
    p_group->setNonEmptyGroupPtr(nonEmptyGroup);
}

void GameSystem::removePlayer(int player_id)
{
    const Player* plr = playersById.getValuePtr(PlayerById(player_id, nullptr))->getPlayerPtr();
    
    playersById.removeNode(PlayerById(player_id, plr));
    plr->getGroupPtr()->removeNode(*plr);
    players.removeNode(*plr);
}


void GameSystem::replaceGroup(int groupId, int replacementId)
{
	if (replacementId <= 0 || groupId <= 0 || groupId == replacementId) //Check if last is valid.
	{
		throw InvalidInput("Invalid input in replaceGroup.");
	}
	bool nonEmpty;
	
	Group *group = this->groups.getValuePtr(groupId),
		*replacement = this->groups.getValuePtr(replacementId);

	nonEmpty = group->getSize() > 0;

	if (nonEmpty)
    {
	    nonEmptyGroups.removeNode(*nonEmptyGroups.getValuePtr(groupId));
    }

	replacement->mergeGroups(*group);
	this->groups.removeNode(*group);
}

int GameSystem::getHighestLevel(int group_id)
{
    if(group_id == 0)
    {
        throw InvalidInput("Invalid input in getHighestLevel.");
    }

    if(group_id < 0)
    {
        if(players.getSize() == 0)
        {
            return -1;
        }
        return players.getHighest().getId();
    }

    Group* group = groups.getValuePtr(group_id);
   
    if(group->getSize() == 0)
    {
        return -1;
    }
    return group->getHighest().getId();
}

int* GameSystem::getAllPlayersByLevel(int group_id, int* numOfPlayers)
{
    if(group_id == 0)
    {
        throw InvalidInput("Invalid input in getAllPlayersByLevel.");
    }

    if(group_id < 0)
    {
        *numOfPlayers = players.getSize(); 
        if(*numOfPlayers == 0)
        {
            return nullptr;
        }

        Player* playersArr = AVLTree<Player>::treeToArray(this->players, true);
        
        return playersToIds(playersArr, *numOfPlayers);
    }

    Group* group = groups.getValuePtr(group_id);

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
    return arr;
}