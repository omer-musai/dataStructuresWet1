#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

#include "AVLTree.hpp"
#include "Player.hpp"
#include "playerById.hpp"
#include "Group.hpp"

class GameSystem
{
    private:
        AVLTree<PlayerById> playersById;
        AVLTree<Player> players;
        AVLTree<Group> groups;
        AVLTree<Group> nonEmptyGroups;


    public:
        void addGroup(int id);
        
        void addPlayer(int player_id, int group_id, int level);
        
        void replaceGroup(int groupId, int replacementId);

        void removePlayer(int player_id);

        int getHighestLevel(int group_id);

        int* getAllPlayersByLevel(int group_id, int* numOfPlayers);

        static int* playersToIds(Player* plrs, int n);

        int* getGroupsHighestLevel(int numOfGroups);
};





#endif //GAME_SYSTEM_H