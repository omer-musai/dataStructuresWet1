#ifndef AVL_H
#define AVL_H

//not sure if necessary
typedef enum
{
    AVL_ALREADY_EXIST,
    AVL_SUCCESS,


} AVLres;


template<class T>
class AVL
{
    private:

        typedef struct node_t
        {
            explicit Node(T element, Node father, int height) : element(element), father(father), height(height), rightSon(nullptr), leftSon(nullptr){}
            int height; //of the sub_tree that this node is the root of
            T element;
            node_t* father;
            node_t* rightSon;
            node_t* leftSon;
        } *Node;


        int numOfElements;
        Node highest;
        Node root;
        
        void RR(T& element);
        void LL(T& element);
        void RL(T& element);
        void LR(T& element); 
        void updateTree(Node current);
        


    public:
        AVLres insert(const T& element);
        AVLres find(const T& element);
        AVLres remove(int id);
        AVLres inOrder();

};

#endif //AVL_H
