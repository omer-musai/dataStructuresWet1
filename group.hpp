#ifndef GROUP_H
#define GROUP_H

#include <memory>

#include "Player.hpp"
#include "AVLTree.hpp"
#include "game_exceptions.hpp"

class Group
{
    private:
        int id;
        std::shared_ptr<AVLTree<Player>> players;
        bool dummy; //Will be used to initialize arrays of Groups.

    public:
        Group() : id(0), players(nullptr), dummy(true) {}

        explicit Group(int id) : players(std::make_shared<AVLTree<Player>>()),
        id(id), dummy(false)
        {
            if(id <= 0)
            {
                throw InvalidInput();
            }
        }

        const Player& getHighest() const
        {
            assert(!dummy);
            return (players->getHighest());
        }

        void removeNode(const Player& plr)
        {
            assert(!dummy);
            players->removeNode(plr);
        }

        int getSize() const
        {
            assert(!dummy);
            return players->getSize();
        }

        void addPlayer(const Player& player)
        {
            assert(!dummy);
            players->addNode(player);
        }

        void mergeGroups(Group& other)
        {
            assert(!dummy);
            //TODO: Ensure stuff get freed here by making some scary leakable Ts.
            std::shared_ptr<AVLTree<Player>> newTree = AVLTree<Player>::mergeTrees(
                *(this->players),
                *(other.players)
            );
            this->players = newTree;
            //other.players->clean(); Should be done by mergeTrees itself.
        }

        std::shared_ptr<AVLTree<Player>> getPlayers() const
        {
            assert(!dummy);
            return (this->players);
        }


		
		bool operator<=(const Group& other) const
		{
            assert(!dummy);
			return this->id <= other.id;
		}
		
		bool operator>(const Group& other) const
		{
            assert(!dummy);
			return !((*this) <= other);
		}
		
		bool operator>=(const Group& other) const
		{
            assert(!dummy);
			return other <= (*this);
		}
		
		bool operator<(const Group& other) const
		{
            assert(!dummy);
			return other > (*this);
		}
		
		bool operator==(const Group& other) const
		{
            assert(!dummy);
			return (*this) <= other && (*this) >= other;
		}

        bool operator!=(const Group& other) const
        {
            assert(!dummy);
            return !(*this == other);
        }
};



#endif //GROUP_H