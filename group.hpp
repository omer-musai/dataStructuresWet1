#ifndef GROUP_H
#define GROUP_H

#include "AVLTree.hpp"
#include "game_exeptions.hpp"
#include "player.hpp"

class Group
{
    private:
        int id;
        AVLTree<Player> players;

    public:
        Group() = delete;

        Group(int id) : id(id), players()
        {
            if(id <= 0)
            {
                throw InvalidInput();
            }
        }

        Player getHighest() const
        {
            return players.getHighest();
        }

        void removeNode(const Player& plr)
        {
            players.removeNode(plr);
        }

        int getSize() const
        {
            return players.getSize();
        }

        void addPlayer(const Player& player)
        {
            players.addNode(player);
        }

        void mergeGroups(Group& other)
        {
            //TODO: Ensure stuff get freed here by making some scary leakable Ts.
            std::shared_ptr<AVLTree<Group>> newTree = AVLTree<Group>::mergeTrees(
                std::shared_ptr<Group>(&(this->players)), std::shared_ptr<Group>(&(other.players))
            );
            this->players = newTree;
            other.players.clean();
        }


		
		bool operator<=(const Group& other) const
		{
			return this->id <= other.id;
		}
		
		bool operator>(const Group& other) const
		{
			return !((*this) <= other);
		}
		
		bool operator>=(const Group& other) const
		{
			return other >= (*this);
		}
		
		bool operator<(const Group& other) const
		{
			return other > (*this);
		}
		
		bool operator==(const Group& other) const
		{
			return (*this) <= other && (*this) >= other;
		}
};



#endif //GROUP_H