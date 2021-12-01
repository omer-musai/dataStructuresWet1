#include "gameSystem.hpp"



void GameSystem::addGroup(int id)
{
    groups.addNode(Group(id));
}

void GameSystem::addPlayer(int player_id, int group_id, int level)
{
    Group* group_ptr = groups.getValuePtr(Group(group_id));

    Player plr(player_id, level, group_ptr);
   
    Player* p = players.addNode(plr);

    playersById.addNode(PlayerById(player_id, p));

    group_ptr->addPlayer(plr);
    
    if(group_ptr->getSize() == 1)
    {
        nonEmptyGroups.addNode(*group_ptr);
    }
}

void GameSystem::removePlayer(int player_id)
{
    Player* plr = playersById.getValuePtr(PlayerById(player_id, nullptr))->getPlayer();
    
    playersById.removeNode(PlayerById(player_id, plr));
    plr->getGroupPtr(true)->removeNode(*plr);
    plr->getGroupPtr(false)->removeNode(*plr);
    players.removeNode(*plr);
}


void GameSystem::replaceGroup(int groupId, int replacementId)
{
	if (replacementId <= 0 || groupId <= 0)
	{
		throw InvalidInput();
	}
	
	Group *group = this->groups.getValuePtr(Group(groupId)),
		*replacement = this->groups.getValuePtr(Group(groupId));
	
	group->mergeGroups(other);
	this->groups.removeNode(*replacement);
}

int GameSystem::getHighestLevel(int group_id)
{
    if(group_id == 0)
    {
        throw InvalidInput();
    }

    if(group_id < 0)
    {
        if(players.getSize() == 0)
        {
            return -1;
        }
        return players.getHighest().getId();
    }

    Group* group = groups.getValuePtr(Group(group_id));
   
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
        throw InvalidInput();
    }

    if(group_id < 0)
    {
        *numOfPlayers = players.getSize(); 
        if(numOfPlayers == 0)
        {
            return nullptr;
        }

        Player* plrs = treeToArray(this->players);
        
        return playersToIds(plrs, *numOfPlayers);
    }

    Group* group = groups.getValuePtr(Group(group_id));

    *numOfPlayers = group->getSize(); 
    if(numOfPlayers == 0)
    {
        return nullptr;
    }

    Player* plrs = AVLTree<Player>::treeToArray(group->players);
    
    return playersToIds(plrs, *numOfPlayers);
}

static int* GameSystem::playersToIds(Player* plrs, int n)
{
    int* arr = malloc(sizeof(int) * n);
    if(!arr)
    {
        throw AllocationError();
    }
    
    for (int i = 0; i < n; i++)
    {
        arr[i] = plrs[i].getId();
    }
    delete[] plrs;
    return arr;
} 

int* GameSystem::getGroupsHighestLevel(int numOfGroups)
{
    if(numOfGroups < 1)
    {
        throw InvalidInput();
    }
    if(numOfGroups < this->nonEmptyGroups.getSize())
    {
        throw Failure();
    }

    int* arr = malloc(sizeof(int) * numOfGroups);
    if(!arr)
    {
        throw AllocationError();
    }

    Group* grps = AVLTree<Player>::treeToArray(this->nonEmptyGroups);
    for(int i = 0; i < numOfGroups; i++)
    {
        arr[i] = grps[i].getHighest().getId();
    }
}