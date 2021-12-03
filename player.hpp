#ifndef PLAYER_H
#define PLAYER_H

#include "game_exceptions.hpp"

class Group;

class Player
{
    private:
        int id;
        int level;
        Group* groupPtr;
        bool dummy; //"Dummy players" will be used to perform searching by key in the tree & initializing arrays.

    public:
        explicit Player(int id = 0) : id(id), level(-1), groupPtr(nullptr), dummy(true) {}
        explicit Player(int id, int level, Group* groupPtr) : id(id), level(level),
            groupPtr(groupPtr), dummy(false)
        {
            if(level < 0 || id <= 0)
            {
                throw InvalidInput();
            }
        }

        int getId() const
        {
            return id;
        }

        int getLevel() const
        {
            return level;
        }

        Group* getGroupPtr() const
        {
            assert(dummy == false);

            return this->groupPtr;
        }

        void setGroupPtr(Group* ptr)
        {
            assert(dummy == false);

            this->groupPtr = ptr;
        }

        bool operator<=(const Player& player) const
        {
            if (dummy)
            {
                return this->id >= player.id;
            }

            if (this->level == player.level)
            {
                //lower id value means higher player value
                return this->id >= player.id;
            }
            else
            {
                return this->level < player.level;
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
            return player > *this;
        }

        bool operator==(const Player& player) const
        {
            assert(dummy || !(this->id == player.id && this->level != player.level));
        
            return this->id == player.id;
        }

        bool operator!=(const Player& player) const
        {
            return !(*this == player);
        }



};


#endif //PLAYER_H