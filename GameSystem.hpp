#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include "AVLTree.hpp"
#include "Player.hpp"
#include "PlayerById.hpp"
#include "Group.hpp"

class GameSystem
{
    private:
        AVLTree<PlayerById> playersById;
        AVLTree<Player> players;
        AVLTree<Group> groups;
        AVLTree<Group> nonEmptyGroups;

        void addPlayer(int playerId, Group* groupPtr, int level);

        static int* playersToIds(Player* player_arr, int n);


    public:
        GameSystem():nonEmptyGroups(false) {}

        void addGroup(int groupId);
        
        void addPlayer(int playerId, int groupId, int level);
        
        void replaceGroup(int groupId, int replacementId);

        void removePlayer(int playerId);

        int getHighestLevel(int groupId);

        int* getAllPlayersByLevel(int groupId, int* numOfPlayers);

        int* getGroupsHighestLevel(int numOfGroups);

        void increaseLevel(int playerId, int levelIncrease);
};





#endif //GAME_SYSTEM_H