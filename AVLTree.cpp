#include "gameSystem.hpp"
#include <iostream>

using std::cout;
using std::endl;

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
    /*MegaPrinter mp;
    TreeValidator tv;

    int arrOdd[] = {1,3,5,7,9},
        arrEven[] = {2,4,6,8,10};
    std::shared_ptr<AVLTree<int>> oddTree = AVLTree<int>::treeFromArray(arrOdd, 5),
        evenTree = AVLTree<int>::treeFromArray(arrEven, 5),
        mergedTree;
    oddTree->inorder(mp); std::cout << std::endl;
    evenTree->inorder(mp); std::cout << std::endl;
    mergedTree = AVLTree<int>::mergeTrees(*evenTree, *oddTree);
    std::cout << "Post-merge:" << std::endl;
    mergedTree->inorder(mp); std::cout << std::endl;
    std::cout << "Highest: " << mergedTree->getHighest() << std::endl;
    mergedTree->inorder(tv);
    tv.printRes();*/

    /*GameSystem system;
    system.addGroup(1);
    system.addGroup(4);
    system.addGroup(3);
    system.addGroup(2);
    system.addPlayer(5, 1, 3);
    system.addPlayer(4, 2, 3);
    system.addPlayer(17, 1, 3);
    std::cout << "Highest: " << system.getHighestLevel(-1) << std::endl;
    std::cout << "Gr. 1 Highest: " << system.getHighestLevel(1) << std::endl;
    std::cout << "Gr. 2 Highest: " << system.getHighestLevel(2) << std::endl;
    int count, *players = system.getAllPlayersByLevel(-1, &count);
    cout << "All players: ";
    for(int cnt = 0; cnt < count; ++cnt) std::cout << players[cnt] << " ";
    free(players);
    players = system.getAllPlayersByLevel(1, &count);
    cout << "\nGr. 1 players: ";
    for(int cnt = 0; cnt < count; ++cnt) std::cout << players[cnt] << " ";
    free(players);
    cout << "\nGr. 1 players post repl.: ";
    system.replaceGroup(2, 1);
    players = system.getAllPlayersByLevel(1, &count);
    for(int cnt = 0; cnt < count; ++cnt) std::cout << players[cnt] << " ";
    free(players);
    cout << "\nGr. 2 players post repl.: ";
    try {
        players = system.getAllPlayersByLevel(2, &count);
        for(int cnt = 0; cnt < count; ++cnt) std::cout << players[cnt] << " ";
        free(players);
    }
    catch (Failure exc) {
        cout << "Gr. 2 was removed :)\n";
    }
    system.addGroup(2);
    system.addPlayer(32, 1, 12);
    system.addPlayer(720, 2, 16);
    system.addPlayer(42, 1, 151);
    players = system.getAllPlayersByLevel(-1, &count);
    for(int cnt = 0; cnt < count; ++cnt) std::cout << players[cnt] << " ";
    free(players);
    cout << endl;

    system.removePlayer(17);
    system.removePlayer(4);
    players = system.getAllPlayersByLevel(-1, &count);
    for(int cnt = 0; cnt < count; ++cnt) std::cout << players[cnt] << " ";
    free(players);*/

    /*GameSystem system;
    system.addGroup(1);
    system.addPlayer(1,1,10);
    system.addPlayer(2,1,100);
    system.addPlayer(3,1,1000);
    system.removePlayer(1);

    int count;
    auto players = system.getAllPlayersByLevel(-1, &count);
    for(int cnt = 0; cnt < count; ++cnt) std::cout << players[cnt] << " ";
    cout << endl;
    free(players);

    system.addGroup(2);
    system.addPlayer(4,2,10000);
    system.replaceGroup(2,1);
    players = system.getAllPlayersByLevel(-1, &count);
    for(int cnt = 0; cnt < count; ++cnt) std::cout << players[cnt] << " ";
    cout << endl;
    free(players);*/

    GameSystem system;
    system.addGroup(1);
    system.addGroup(2);
    system.addGroup(3);
    system.addPlayer(1,1,10);
    system.addPlayer(2,1,100);
    system.addPlayer(3,1,1000);
    system.removePlayer(3);
    system.addPlayer(4,2,500);
    system.addPlayer(5,3,500);
    system.addPlayer(6,3,600);
    system.removePlayer(6);
    system.addPlayer(6,3,1600);
    int* players = system.getGroupsHighestLevel(3);
    for(int cnt = 0; cnt < 3; ++cnt) std::cout << players[cnt] << " ";
    cout << endl;
    free(players);

    cout << "\nIncreasing #5's level by 10,000, and re-printing highest by groups:\n";
    system.increaseLevel(5, 10000);
    players = system.getGroupsHighestLevel(3);
    for(int cnt = 0; cnt < 3; ++cnt) std::cout << players[cnt] << " ";
    cout << endl;
    free(players);


    /*GameSystem system;
    system.addGroup(1);
    system.addGroup(2);
    system.addGroup(3);
    system.addPlayer(1, 1, 1);
    system.addPlayer(2,2,2);
    system.addPlayer(3,3,3); //Leak happens once we have 3 groups and remove the middle one.
    system.removePlayer(2);*/

    return 0;
}
