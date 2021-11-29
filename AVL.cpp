#include "AVL.h"

template<class T>
AVLres AVL<T>::insert(const T& element)
{
    Node current = this->root;
    if(current == nullptr)
    {
        Node new_node = new Node(element, nullptr, 0);
        return AVL_SUCCESS;
    }
  
    while(true)
    {
        if(current->element < element)
        {
            if(current->leftSon == nullptr)
            {
                Node new_node = new Node(element, current, 0);
                current->leftSon = new_node;
                break;
            }
            
            current = current->leftSon;
        }

        else if(current->element > element)
        {
            if(current->rightSon == nullptr)
            {
                Node new_node = new Node(element, current, 0);
                current->rightSon = new_node;
                break;
            }
            
            current = current->rightSon;
        }

        else //already exist
        {
            return AVL_ALREADY_EXIST;
        }

    }

    this->updateTree(current);
    
    return AVL_SUCCESS;
}



template<class T>
void AVL<T>::updateTree(Node current)
{
    int h_right, h_left, h_curr, BF;
    
    while(current != this->root)
    {
        if(current->leftSon == nullptr)
        {
            h_left = 0;
        }
        else
        {
            h_left = current->leftSon->height;
        }
        if(current->rightSon == nullptr)
        {
            h_right = 0;
        }
        else
        {
            h_right = current->rightSon->height;
        }

        h_curr = ( (h_right - h_left > 0) ? (h_right + 1) : (h_left + 1) );
        //if height of the sub_tree which current is the root of, didn't changed-  we are good to go
        if(h_curr == current->height)
        {
            return;
        }
        //update current height
        current->height = h_curr;
        
        BF = ( (h_right - h_left) > 0 ? (h_right - h_left) : (h_left - h_right) );


        if(BF == 2)
        {
            //ROLLs
        }

        current = current->father;
    }
    
}