#ifndef GROUP_H
#define GROUP_H

#include "AVL.h"


class Group
{
    private:
        int id;
        AVL<Player> PT;

    public:
        Group() = delete;
        Group(int id, AVL<Player> PT) : id(id), PT(PT) {}

        int compare(const Group& group) const
        {
            if(this->id == group.id)
            {
                return 0;
            }
            else
            {
                return this->id < group.id ? 1 : -1;
            }
        }
};



#endif //GROUP_H