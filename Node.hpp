#ifndef AVLTREE_BIDIRECTIONAL_NODE
#define AVLTREE_BIDIRECTIONAL_NODE

//A bidirectional node to be used for an AVL tree implementation.
template <class T>
class Node
{
private:
    T value;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    int height;
public:
    explicit Node(const T& value)
    {
        this->value = value;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        height = 0;
    }

    Node<T>& operator=(Node<T>& other) = delete;

    Node() = delete;

    void setLeft(Node<T>* left)
    {
        this->left = left;
        if (left != nullptr)
        {
            left->parent = this;
        }
    }

    void setRight(Node<T>* right)
    {
        this->right = right;
        if (right != nullptr)
        {
            right->parent = this;
        }
    }

    void setParent(Node<T>* parent)
    {
        this->parent = parent;
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

    void setHeight(int height)
    {
        this->height = height;
    }

    void incrementHeight()
    {
        ++height;
    }

    void decrementHeight()
    {
        --height;
    }

    int getHeight() const
    {
        return height;
    }

    const T& getValue() const
    {
        return value;
    }

    void setValue(const T& value)
    {
        this->value = value;
    }

    int getLeftHeight() const
    {
        return left == nullptr ? -1 : left->height;
    }

    int getRightHeight() const
    {
        return right == nullptr ? -1 : right->height;
    }

    bool isRoot() const
    {
        return parent == nullptr;
    }

    ~Node() = default;
};

#endif //AVLTREE_NODE
