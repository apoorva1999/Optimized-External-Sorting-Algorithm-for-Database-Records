#include "Iterator.h"
#include<queue>
class Node {
    public: 
    int wid;
    int lid;
};

class Tree {
    public:
    static vector<Node> tree;
    static void generateRuns(); 
    static void mergeSortedRuns();
    static void buildTree();
    static Row getWinner() ;

    private:
    static bool compare(int i1, int i2);
    static void replacementSelection(int idx);
};

