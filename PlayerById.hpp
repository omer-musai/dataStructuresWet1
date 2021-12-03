#ifndef PLAYER_BY_ID_H
#define PLAYER_BY_ID_H

#include "Group.hpp"
#include "game_exceptions.hpp"
#include "Player.hpp"
class PlayerById
{
    private:
        int id;
        const Player* player;

    public:
        PlayerById() = delete;
        explicit PlayerById(int id, const Player* player) : id(id), player(player)
        {
            if(id <= 0)
            {
                throw InvalidInput();
            }
        }

        int getId() const {return id;}
        
        const Player* getPlayerPtr()
        {
            return player;
        }
        
        bool operator<=(const PlayerById& other) const
		{
			return this->id >= other.id;
		}
		
		bool operator>(const PlayerById& other) const
		{
			return !((*this) <= other);
		}
		
		bool operator>=(const PlayerById& other) const
		{
			return other <= (*this);
		}
		
		bool operator<(const PlayerById& other) const
		{
			return other > (*this);
		}
		
		bool operator==(const PlayerById& other) const
		{
			return (*this) <= other && (*this) >= other;
		}

		bool operator!=(const PlayerById& other) const
        {
            return !(*this == other);
        }
};



#endif //PLAYER_BY_ID_H