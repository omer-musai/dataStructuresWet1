#include "AVLTree.hpp"
#include <iostream>

class Printer
{
public:
    template <class T>
    void operator()(T value, int height, T parentValue, T leftValue, T rightValue, int BF)
    {
        std::cout << "(V:" << value << ";H:" << height << ") ";
    }
};

class MegaPrinter
{
public:
    template <class T>
    void operator()(T value, int height, T leftValue, T rightValue, T parentValue, int BF)
    {
        std::cout << value << ": H:" << height << ", P:" << parentValue << ", L:" << leftValue << ", R:" << rightValue << "\n";
    }
};

class TreeValidator
{
private:
    int violations = 0;
public:
    template <class T>
    void operator()(T value, int height, T leftValue, T rightValue, T parentValue, int BF)
    {
        violations += (BF > 0 ? BF : -BF) > 1;
    }
    void printRes()
    {
        std::cout << ((violations > 0) ? "Not a valid AVL tree." : "Valid AVL tree.");
        violations = 0;
    }
};

int main() {
    /*Printer p;
    MegaPrinter mp;
    TreeValidator tv;
    AVLTree<int> tree;
    tree.addNode(10);
    tree.addNode(20);
    tree.addNode(30);
    tree.addNode(-50);
    tree.addNode(-60);
    tree.addNode(0);
    tree.addNode(15);
    tree.addNode(-7);
    tree.removeNode(30);
    tree.addNode(-8);
    tree.addNode(5);
    tree.addNode(6);
    tree.addNode(7);
    tree.addNode(8);
    tree.removeNode(8);
    tree.removeNode(20);
    tree.removeNode(5);
    tree.removeNode(7);
    tree.inorder(p);
    tree.inorder(mp);
    tree.inorder(tv);
    tv.printRes();
    std::cout << "\nHighest: " << tree.getHighest() << std::endl;*/

    /*int arr[] = {1,2,3,4,5,6,7,8,9,10};
    std::shared_ptr<AVLTree<int>> tree = AVLTree<int>::treeFromArray(arr, 10);
    MegaPrinter mp;
    TreeValidator tv;
    tree->inorder(mp);
    tree->inorder(tv);
    tv.printRes();*/

    MegaPrinter mp;
    TreeValidator tv;

    int arrOdd[] = {1,3,5,7,9},
        arrEven[] = {2,4,6,8,10};
    std::shared_ptr<AVLTree<int>> oddTree = AVLTree<int>::treeFromArray(arrOdd, 5),
        evenTree = AVLTree<int>::treeFromArray(arrEven, 5),
        mergedTree;
    oddTree->inorder(mp); std::cout << std::endl;
    evenTree->inorder(mp); std::cout << std::endl;
    mergedTree = AVLTree<int>::mergeTrees(evenTree, oddTree);
    std::cout << "Post-merge:" << std::endl;
    mergedTree->inorder(mp); std::cout << std::endl;
    std::cout << "Highest: " << mergedTree->getHighest() << std::endl;
    mergedTree->inorder(tv);
    tv.printRes();


    return 0;
}
