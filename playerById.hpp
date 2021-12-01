#ifndef PLAYER_BY_ID_H
#define PLAYER_BY_ID_H

#include "group.hpp"
#include "game_exeptions.hpp"
#include "player.hpp"
class PlayerById
{
    private:
        int id;
        Player* player;

    public:
        PlayerById() = delete;
        explicit PlayerById(int id, Player* player) : id(id), player(player)
        {
            if(id <= 0)
            {
                throw InvalidInput();
            }
        }
        
        Player* getPlayer()
        {
            return player;
        }
        
        bool operator<=(const PlayerById& other)
		{
			return this->id <= other.id;
		}
		
		bool operator>(const PlayerById& other)
		{
			return !((*this) <= other);
		}
		
		bool operator>=(const PlayerById& other)
		{
			return other ->= (*this);
		}
		
		bool operator<(const PlayerById& other)
		{
			return other > (*this);
		}
		
		bool operator==(const PlayerById& other)
		{
			return (*this) <= other && (*this) >= other;
		}
};



#endif //PLAYER_BY_ID_H