#ifndef AVLTree_HPP
#define AVLTree_HPP

#define AVL_BALANCE_BOUND 1

#include "Node.hpp"
#include "game_exceptions.hpp"

#include <cassert>
#include <memory>

template <class T>
class AVLTree
{
private:
    Node<T> *root;
    Node<T> *highest;
    int nodeCount;
    const bool clone;

    //Static utilities: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    class StaticAVLUtilities
    {
        friend class AVLTree;
        class ArrayFromTreePopulator {
        private:
            T** array;
            int size;
            int index;
            bool reverse;

        public:
            ArrayFromTreePopulator() = delete;
            explicit ArrayFromTreePopulator(T** array, int size = -1, bool reverse=false):
                    array(array), size(size), index(0), reverse(reverse)
            {
                if (reverse)
                {
                    if (size <= 0)
                    {
                        throw InvalidInput("Populator: reversing is only possible if size is provided.");
                    }
                    index = size - 1;
                }
            }

            void operator()(T& value, int height, T parentValue, T leftValue, T rightValue, int BF) {
                if (reverse)
                {
                    if(size < 0 || index >= 0)
                    {
                        array[index--] = &value;
                    }
                }
                else
                {
                    if (size < 0 || index < size)
                    {
                        array[index++] = &value;
                    }
                }
            }
        };

        static T** treeToArray(const AVLTree<T>& tree, bool reverse=false) {
            T** array = new T*[tree.getSize()];
            try
            {
                ArrayFromTreePopulator populator(array, tree.getSize(), reverse);
                tree.inorder(populator);
            }
            catch (std::exception& exception)
            {
                delete[] array;
                throw exception;
            }
            return array;
        }

        static T** arrayMerge(T** arr1, int size1, T** arr2, int size2) {
            T** array = new T*[size1 + size2];
            try
            {
                int i1 = 0, i2 = 0, i = 0;
                while (i1 < size1 && i2 < size2) {
                    array[i] = (*arr1[i1]) < (*arr2[i2]) ? arr1[i1] : arr2[i2];
                    if (array[i] == arr1[i1]) {
                        ++i1;
                    } else {
                        ++i2;
                    }
                    ++i;
                }
                while (i1 < size1) {
                    array[i++] = arr1[i1++];
                }
                while (i2 < size2) {
                    array[i++] = arr2[i2++];
                }
            }
            catch (std::out_of_range& exception)
            {
                delete[] array;
            }

            return array;
        };

        //This uses the algorithm described & proved in the doc.
        static std::shared_ptr<AVLTree<T>> AVLFromArray(T** arr, int size, bool clone=true) {
            assert(size >= 0);

            std::shared_ptr<AVLTree<T>> tree = std::shared_ptr<AVLTree<T>>(new AVLTree<T>(clone));
            if (size > 0)
            {
                int m = (size % 2 == 0 ? size / 2 : (size + 1) / 2) - 1; //m=ceil(size/2)-1
                tree->addNode(arr[m]); //O(1) since the tree is empty.

                if (size == 2) {
                    tree->addNode(arr[m + 1]); //O(1) since the tree is always sized 1 and this point.
                } else if (size > 2) {
                    tree->setLeftSubtree(AVLFromArray(arr, m, clone));
                    tree->setRightSubtree(AVLFromArray(arr + (m + 1), size - m - 1, clone));
                }
            }

            return tree;
        }

        //THIS RUINS THE PARAMETER TREES. Careful!
        static std::shared_ptr<AVLTree<T>> mergeTrees(AVLTree<T>& t1, AVLTree<T>& t2, bool clone=true) {
            T **t1arr = nullptr, **t2arr = nullptr, **merged = nullptr;
            int totalSize = t1.getSize() + t2.getSize();
            std::shared_ptr<AVLTree<T>> result;
            try {
                t1arr = treeToArray(t1);
                t2arr = treeToArray(t2);
                merged = arrayMerge(t1arr, t1.getSize(), t2arr, t2.getSize());
                result = treeFromArray(merged, totalSize, clone);
                t1.clean();
                t2.clean();
            }
            catch (std::exception &exception) {

            }

            if (t1arr != nullptr)
            {
                delete[] t1arr;
            }
            if (t2arr != nullptr)
            {
                delete[] t2arr;
            }
            if (merged != nullptr)
            {
                delete[] merged;
            }

            return result;
        }
        //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    };


    enum Order { smaller, equal, larger };

    int abs(int a) { return a > 0 ? a : -a; }

    void setLeftSubtree(std::shared_ptr<AVLTree<T>> tree)
    {
        assert(root != nullptr);
        root->setLeft(tree->root);
        tree->root = nullptr; //To ensure no scary O(n) cleaning operations take place.
        root->updateHeight();
        nodeCount = tree->nodeCount + nodeCount;
    }

    void setRightSubtree(std::shared_ptr<AVLTree<T>> tree)
    {
        assert(root != nullptr);
        root->setRight(tree->root);
        tree->root = nullptr; //To ensure no scary O(n) cleaning operations take place.
        if (tree->highest != nullptr)
        {
            this->highest = tree->highest;
        }
        root->updateHeight();
        nodeCount = tree->nodeCount + nodeCount;
    }

    void freeList()
    {
        freeListAux(root);
    }

    void freeListAux(Node<T>* curr)
    {
        if (curr == nullptr) return;
        freeListAux(curr->getLeft());
        freeListAux(curr->getRight());
        delete curr;
    }

    Node<T>* findLocation(const T& value, Order &orderRel)
    {
        if (root == nullptr) return nullptr;
        return findLocationAux(value, root, orderRel);
    }

    Node<T>* findLocationAux(const T& value, Node<T>* curr, Order &orderRel)
    {
        if (value == curr->getValue())
        {
            orderRel = equal;
            return curr;
        }

        bool isLarger = value > curr->getValue();
        Node<T>* nextNode = isLarger ? curr->getRight() : curr->getLeft();

        if (nextNode == nullptr)
        {
            orderRel = isLarger ? larger : smaller;
            return curr;
        }

        return findLocationAux(value, nextNode, orderRel);
    }

    template <class K>
    Node<T>* findByKey(K key, bool* found)
    {
        T value(key);
        Order orderRel;
        Node<T>* node = findLocation(value, orderRel);
        if (found != nullptr)
        {
            *found = !(node == nullptr || orderRel != equal);
        }
        return node;
    }

    //Updates heights and returns the lowest node with an invalid balance factor.
    void updateTree(Node<T>* node, bool continueToRoot=false)
    {
        if (node == nullptr) return;

        node->updateHeight();
        Node<T>* nextNode = rotate(node);

        Node<T>* parent = nextNode->getParent();

        if (!continueToRoot && (parent == nullptr || parent->getHeight() >= node->getHeight() + 1))
        {
            return;
        }

        updateTree(parent);

    }

    int getBalanceFactor(Node<T>* node = nullptr) const
    {
        if (node == nullptr) node = root;
        return node->getLeftHeight() - node->getRightHeight();
    }

    template <class A>
    void inorderAux(A& action, Node<T>* curr) const
    {
        if (curr == nullptr) return;
        inorderAux(action, curr->getLeft());
        action(
            curr->getValue(),
            curr->getHeight(),
            curr->getLeft() == nullptr ? T() : curr->getLeft()->getValue(),
            curr->getRight() == nullptr ? T() : curr->getRight()->getValue(),
            curr->getParent() == nullptr ? T() : curr->getParent()->getValue(),
            getBalanceFactor(curr)
        );
        inorderAux(action, curr->getRight());
    }

    /*
     * addNode auxiliary method adding node given findLocation's return values.
     */
    void addNodeToLocation(Node<T>* location, Node<T>* newNode, Order orderRel)
    {
        if (orderRel == equal)
        {
            //Trying to add a node that was already in the tree.
            throw Failure("Trying to add a node that was already added.");
        }

        if (orderRel == larger)
        {
            assert(location->getRight() == nullptr);
            location->setRight(newNode);
        }
        else
        {
            assert(location->getLeft() == nullptr);
            location->setLeft(newNode);
        }
    }

    Node<T>* LLRotation(Node<T>* subtreeRoot)
    {
        Node<T> *oldRoot = subtreeRoot,
            *rootParent = subtreeRoot->getParent(),
            *newRoot = subtreeRoot->getLeft();

        //Set new root:
        if (rootParent != nullptr)
        {
            if (rootParent->getValue() > oldRoot->getValue())
            {
                rootParent->setLeft(newRoot);
            }
            else
            {
                rootParent->setRight(newRoot);
            }
        }
        else
        {
            root = newRoot;
            newRoot->setParent(nullptr);
        }

        //Pluck new root's old right child, and make it old root's new left:
        oldRoot->setLeft(newRoot->getRight());

        //Make old root the right child of the new root:
        newRoot->setRight(oldRoot);

        //Update the height that was affected:
        oldRoot->updateHeight();

        return newRoot;
    }

    Node<T>* RRRotation(Node<T>* subtreeRoot)
    {
        Node<T> *oldRoot = subtreeRoot,
                *rootParent = subtreeRoot->getParent(),
                *newRoot = subtreeRoot->getRight();

        //Set new root:
        if (rootParent != nullptr)
        {
            if (rootParent->getValue() > oldRoot->getValue())
            {
                rootParent->setLeft(newRoot);
            }
            else
            {
                rootParent->setRight(newRoot);
            }
        }
        else
        {
            root = newRoot;
            newRoot->setParent(nullptr);
        }

        //Pluck new root's old left child, and make it old root's new right:
        oldRoot->setRight(newRoot->getLeft());

        //Make old root the left child of the new root:
        newRoot->setLeft(oldRoot);

        //Update the height that was affected:
        oldRoot->updateHeight();

        return newRoot;
    }

    Node<T>* LRRotation(Node<T>* subtreeRoot)
    {
        Node<T> *rootParent = subtreeRoot->getParent(),
                *newRight = subtreeRoot,
                *newLeft = newRight->getLeft(),
                *newRoot = newLeft->getRight();

        //Set new root:
        if (rootParent != nullptr)
        {
            if (rootParent->getValue() > subtreeRoot->getValue())
            {
                rootParent->setLeft(newRoot);
            }
            else
            {
                rootParent->setRight(newRoot);
            }
        }
        else
        {
            root = newRoot;
            newRoot->setParent(nullptr);
        }

        //Rotation fun stuff:
        newRight->setLeft(newRoot->getRight());
        newLeft->setRight(newRoot->getLeft());
        newRoot->setLeft(newLeft);
        newRoot->setRight(newRight);

        //Update the heights that were affected:
        newRight->updateHeight();
        newLeft->updateHeight();
        newRoot->updateHeight();

        return newRoot;
    }

    Node<T>* RLRotation(Node<T>* subtreeRoot)
    {
        Node<T> *rootParent = subtreeRoot->getParent(),
                *newLeft = subtreeRoot,
                *newRight = newLeft->getRight(),
                *newRoot = newRight->getLeft();

        //Set new root:
        if (rootParent != nullptr)
        {
            if (rootParent->getValue() > subtreeRoot->getValue())
            {
                rootParent->setLeft(newRoot);
            }
            else
            {
                rootParent->setRight(newRoot);
            }
        }
        else
        {
            root = newRoot;
            newRoot->setParent(nullptr);
        }

        //Rotation fun stuff:
        newRight->setLeft(newRoot->getRight());
        newLeft->setRight(newRoot->getLeft());
        newRoot->setLeft(newLeft);
        newRoot->setRight(newRight);

        //Update the heights that were affected:
        newRight->updateHeight();
        newLeft->updateHeight();
        newRoot->updateHeight();

        return newRoot;
    }

    /*
     * Performs rotation based on subtreeRoot's balance factor and that of its children.
     * @param recursive determines if this should happen all the way up to the root.
     *        Should be set to true if rotate is called for node removal.
     * Returns the new root of the subtree post-rotation.
     */
    Node<T>* rotate(Node<T>* subtreeRoot, bool recursive=false)
    {
        Node<T>* newRoot = subtreeRoot;
        int rootBF = getBalanceFactor(subtreeRoot);
        if (abs(rootBF) > AVL_BALANCE_BOUND)
        {
            //rootBF is either 2==AVL_BALANCE_BOUND+1 (for Lx rotations) or minus that (for Rx rotations).
            assert(rootBF == (AVL_BALANCE_BOUND + 1) || rootBF == -(AVL_BALANCE_BOUND + 1));
            if (rootBF == AVL_BALANCE_BOUND + 1)
            {
                //Lx rotations
                assert(abs(getBalanceFactor(subtreeRoot->getLeft())) <= 1);

                if (getBalanceFactor(subtreeRoot->getLeft()) >= 0)
                {
                    newRoot = LLRotation(subtreeRoot);
                }
                else
                {
                    newRoot = LRRotation(subtreeRoot);
                }
            }
            else
            {
                //Rx rotations
                assert(abs(getBalanceFactor(subtreeRoot->getRight())) <= 1);
                if (getBalanceFactor(subtreeRoot->getRight()) <= 0)
                {
                    newRoot = RRRotation(subtreeRoot);
                }
                else
                {
                    newRoot = RLRotation(subtreeRoot);
                }
            }

            //This is for the case of node removal.
            if (recursive && newRoot->getParent() != nullptr)
            {
                return rotate(newRoot->getParent());
            }
        }
        return newRoot;
    }

    /*
     * Finds the next in order (inorder) in the specific relevant case within removeNode.
     */
    Node<T>* removeNode_nextInorderAux(Node<T>* node)
    {
        assert(node != nullptr && node->getRight() != nullptr);
        node = node->getRight();
        while (node->getLeft() != nullptr)
        {
            node = node->getLeft();
        }
        return node;
    }

    void updatePtr(T* ptr)
    {
        if (clone)
        {
            throw Failure("Can't update pointers in clone mode.");
        }

        Order orderRel;
        Node<T>* node = findLocation(*ptr, orderRel);
        if (orderRel != equal)
        {
            throw Failure("Tried to fetch non-existent node in updatePtr.");
        }
        node->setValue(ptr);
    }

    /*
     * Finds the previous in order (inorder) of highest in the specific relevant case within removeNode.
     */
    void removeNode_updateHighestAux(Node<T>* node)
    {
        assert(node != nullptr && highest != nullptr);
        if (node->getValue() != highest->getValue())
        {
            return;
        }
        if (node->getLeft() != nullptr)
        {
            highest = node->getLeft();
            while (highest->getRight() != nullptr)
            {
                highest = highest->getRight();
            }
        }
        else if (node->getParent() != nullptr)
        {
            highest = node->getParent();
        }
        else
        {
            highest = nullptr;
        }
    }

    /*
     * Aux of removeNode to remove a leaf. Returns its parent, or nullptr if it was the root.
     */
    Node<T>* removeNode_LeafAux(Node<T>* node)
    {
        assert (node != nullptr && node->getHeight() == 0);
        Node<T>* parent = node->getParent();
        if (parent != nullptr)
        {
            //Can't just check if value > parent value because sorting may momentarily be broken
            //after using swapNodes for node removal.
            //if (parent->getLeft() != nullptr && node->getValue() == parent->getLeft()->getValue())
            if (parent->getLeft() == node)
            {
                parent->setLeft(nullptr);
            }
            else
            {
                parent->setRight(nullptr);
            }
        }
        else
        {
            root = nullptr;
        }
        delete node;
        return parent;
    }

    /*
     * Aux of removeNode to remove a node with only one child. Returns its parent or nullptr if root.
     */
    Node<T>* removeNode_OneChildAux(Node<T>* node)
    {
        assert (node->getLeft() == nullptr || node->getRight() == nullptr);
        Node<T> *parent = node->getParent(), *child = node->getLeft();
        if (child == nullptr) child = node->getRight();

        if (parent == nullptr)
        {
            root = child;
            child->setParent(nullptr);
        }
        else
        {
            if (parent->getLeft() == node)
            {
                parent->setLeft(child);
            }
            else
            {
                parent->setRight(child);
            }
        }
        delete node;
        return parent;
    }

    /*
     * Removes the node as though it's a normal binary search tree.
     * Returns the lowest node affected.
     */
    Node<T>* removeNodeAux(Node<T>* node)
    {
        assert (node != nullptr);

        //Leaf:
        if (node->getHeight() == 0)
        {
            return removeNode_LeafAux(node);
        }
            //Only one descendant:
        else if (node->getLeft() == nullptr || node->getRight() == nullptr)
        {
            return removeNode_OneChildAux(node);
        }
        else
        {
            //Swap node with its successor in the inorder sense:
            Node<T>* nextInOrder = removeNode_nextInorderAux(node);
            swapNodes(node, nextInOrder);

            //We're now looking at a node which either only has one descendant (since no left child),
            //or is a leaf.
            //Call the function again to enter one of the previous blocks.
            return removeNodeAux(nextInOrder);
        }
    }

    void swapNodes(Node<T>* a, Node<T>* b)
    {
        assert(a != nullptr && b != nullptr);

        //Swapping highest ptr too if needed:
        if (highest->getValue() == a->getValue())
        {
            highest = b;
        }
        else if (highest->getValue() == b->getValue())
        {
            highest = a;
        }

        //If highest is root then it can only have at most 1 left child, so we won't get here.
        //if ()

        a->swap(b);
    }

public:
    explicit AVLTree(bool clone=true):root(nullptr),highest(nullptr),nodeCount(0),clone(clone)
    {}

    /*
     * Adds a new node to the tree, keeping it a valid AVL one.
     * Potentially throws a memory allocation error.
     */
    void removeNode(const T& value)
    {
        Order orderRel;
        Node<T>* node = findLocation(value, orderRel);
        if (node == nullptr || orderRel != equal)
        {
            //Node isn't in the tree.
            throw Failure("Tried to remove non-existent node.");
        }
        removeNode_updateHighestAux(node);
        Node<T>* nodeToFix = removeNodeAux(node);
        --nodeCount;

        updateTree(nodeToFix, true);
    }

    //TODO: Make sure no functions are exposed when they shouldn't be.

    T* addNode(T* value)
    {
        Node<T>* newNode(new Node<T>(*value, clone));
        try
        {
            if (root == nullptr)
            {
                root = highest = newNode;
            }
            else
            {
                Order orderRel;
                Node<T>* location = findLocation(*value, orderRel);
                addNodeToLocation(location, newNode, orderRel);

                updateTree(newNode);

                assert(highest != nullptr);
                if (highest->getValue() < newNode->getValue())
                {
                    highest = newNode;
                }
            }

            ++nodeCount;
            return &(newNode->getValue());
        }
        catch (Failure& exception)
        {
            delete newNode;
            throw exception;
        }
    }

    const T& getHighest() const
    {
        return highest->getValue();
    }

    template <class A>
    void inorder(A& action) const
    {
        inorderAux(action, root);
    }

    int getSize() const
    {
        return this->nodeCount;
    }

    /*
     * This function emulates searching an object by key. (For example: getValuePtr(Group(3)) to find group with id==3.)
     */
    template <class K>
    T* getValuePtr(K key)
    {
        bool found;
        Node<T>* node = findByKey(key, &found);
        if (!found)
        {
            throw Failure("Tried to fetch non-existent node.");
        }
        return &(node->getValue());
    }

    template <class K>
    bool isInTree(K key)
    {
        bool found;
        findByKey(key, &found);
        return found;
    }

    static std::shared_ptr<AVLTree<T>> treeFromArray(T** arr, int size, bool clone=true)
    {
        return StaticAVLUtilities::AVLFromArray(arr, size, clone);
    }

    static T** treeToArray(const AVLTree<T>& tree, bool reverse=false)
    {
        return StaticAVLUtilities::treeToArray(tree, reverse);
    }

    /*
     * THIS RUINS THE PARAMETER TREES! CAREFUL!
     */
    static std::shared_ptr<AVLTree<T>> mergeTrees(AVLTree<T>& t1, AVLTree<T>& t2, bool clone=true)
    {
        return StaticAVLUtilities::mergeTrees(t1, t2, clone);
    }

    void clean()
    {
        this->freeList();
        this->root = nullptr;
        this->highest = nullptr;
        this->nodeCount = 0;
    }

    AVLTree(AVLTree& other) = delete;
    AVLTree& operator=(AVLTree& other) = delete;
    ~AVLTree()
    {
        freeList();
    }
};

#endif //AVLTree_HPP 