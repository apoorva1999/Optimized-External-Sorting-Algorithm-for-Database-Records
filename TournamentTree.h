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
    static void buildTree(int n, vector<queue<Row> > &runs);
    static Row getWinner(vector<queue<Row> > &runs) ;

    private:
    static bool compare(int i1, int i2);
    static void replacementSelection(vector<queue<Row> > &runs, int idx);


};

