#ifndef PLAYER_H
#define PLAYER_H

#include "group.hpp"
#include "game_exeptions.hpp"
class Player
{
    private:
        int id;
        int level;
        Group* groupPtr;
        Group* nonEmptyGroupPtr;

    public:
        Player() = delete;
        explicit Player(int id, int level, Group* groupPtr) : id(id), level(level) ,groupPtr(groupPtr)
        {
            if(level < 0 || id <= 0)
            {
                throw InvalidInput();
            }
        }
       
        void increaseLevel(int n)
        {
            if(n <= 0)
            {
                throw InvalidInput();
            }
            
            this->level += n;
        }

        int getId()
        {
            return id;
        }

        Group* getGroupPtr(bool mainTree = true)
        {
            return mainTree ? this->groupPtr : this->nonEmptyGroupPtr;
        }

        bool operator<=(const Player& player) const
        {
            if (this->level == player.level)
            {
                //lower id value means higher player value
                return this->id >= player.id ? true : false;
            }
            else
            {
                return this->level < player.level ? true : false;
            }
        }
        bool operator>=(const Player& player) const
        {
            return player <= *this;
        }

        bool operator>(const Player& player) const
        {
            return !(*this <= player);
        }

        bool operator<(const Player& player) const
        {
            return !(*this >= player);
        }

        bool operator==()(const Player& player) const
        {
            assert(this->id == player.id && this->level != player.level);
        
            return this->id == player.id;

        }



};


#endif //PLAYER_H