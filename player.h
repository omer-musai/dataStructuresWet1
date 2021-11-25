#ifndef PLAYER_H
#define PLAYER_H

#include "group.h"
class Player
{
    private:
        int id;
        int groupId;
        int level;
        Group* groupPtr;

    public:
        Player() = delete;
        Player(int id, int groupId, int level, Group* groupPtr) : id(id), groupId(groupId), level(level) ,groupPtr(groupPtr){}
       
        int compare(const Player& player) const
        { 
            if (this->id == player.id)
            {
                return 0;
            } 

            if (this->level == player.level)
            {
                //lower id value means higher player value
                return this->id < player.id ? 1 : -1;
            }
            else
            {
                return this->level > player.id ? 1 : -1;
            }
        }
};


#endif //PLAYER_H