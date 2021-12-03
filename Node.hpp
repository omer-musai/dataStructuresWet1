#ifndef AVLTREE_BIDIRECTIONAL_NODE
#define AVLTREE_BIDIRECTIONAL_NODE

#include <cassert>

//A bidirectional node to be used for an AVL tree implementation.
template <class T>
class Node
{
private:
    T* value;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    int height;
    bool clone;

public:
    explicit Node(T& value, bool clone=true) : left(nullptr), right(nullptr), parent(nullptr), height(0), clone(clone), value(nullptr)
    {
        this->setValue(value);
    }

    Node<T>& operator=(Node<T>& other) = delete;

    Node() = delete;

    void setLeft(Node<T>* newLeft)
    {
        this->left = newLeft;
        if (newLeft != nullptr)
        {
            newLeft->parent = this;
        }
    }

    void setRight(Node<T>* newRight)
    {
        this->right = newRight;
        if (newRight != nullptr)
        {
            newRight->parent = this;
        }
    }

    void setParent(Node<T>* newParent)
    {
        this->parent = newParent;
    }

    Node<T>* getLeft()
    {
        return left;
    }

    Node<T>* getRight()
    {
        return right;
    }

    Node<T>* getParent()
    {
        return parent;
    }

    void updateHeight()
    {
        int lh = this->getLeftHeight(), rh = this->getRightHeight();
        this->height = 1 + (lh > rh ? lh : rh);
    }

    int getHeight() const
    {
        return height;
    }

    T& getValue() const
    {
        return *value;
    }

    void setValue(T& newValue)
    {
        if (clone && value != nullptr)
        {
            delete value;
        }
        this->value = clone ? new T(newValue) : &newValue;
    }

    int getLeftHeight() const
    {
        return left == nullptr ? -1 : left->height;
    }

    int getRightHeight() const
    {
        return right == nullptr ? -1 : right->height;
    }

    void swap(Node<T>* other)
    {
        T* temp = other->value;
        other->value = value;
        value = temp;
    }

    ~Node()
    {
        if (value != nullptr && clone)
        {
            delete value;
        }
    }
};

#endif //AVLTREE_NODE
