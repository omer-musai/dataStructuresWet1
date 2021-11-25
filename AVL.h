#ifndef AVL_H
#define AVL_H

//not sure if necessary
typedef enum
{

} AVLres;


template<class T>
class AVL
{
    private:

        typedef struct node_t
        {
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


    public:
        AVLres insert(const T& element);
        AVLres remove(int id);
        AVLres inOrder();

};

#endif //AVL_H
