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

        class GroupModifier
        {
            private:
                Group* ptr;

            public:
                GroupModifier() = delete;
                explicit GroupModifier(Group* ptr):ptr(ptr)
                {}

                void operator()(Player& value, int height, Player parentValue, Player leftValue, Player rightValue, int BF)
                {
                    value.setGroupPtr(ptr);
                }
        };

    public:
        Group() : id(0), players(nullptr), dummy(true) {}

        explicit Group(int id) : players(std::make_shared<AVLTree<Player>>(false)),
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
            players->removeNode(plr);
        }

        int getSize() const
        {
            assert(!dummy);
            return players->getSize();
        }

        Player* addPlayer(Player& player)
        {
            assert(!dummy);
            return players->addNode(&player);
        }

        void mergeGroups(Group& other)
        {
            assert(!dummy);
            //TODO: Ensure stuff get freed here by making some scary leakable Ts.
            GroupModifier gm(this);
            other.players->inorder(gm); //TODO: note in .lyx that this is fine complexity-wise.

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