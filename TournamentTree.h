#include "Iterator.h"
#include<queue>
class Node {
    public: 
    int wid;
    int lid;
};

class Tree {
    public:
    vector<Node> tree;
    void generateRuns(Memory &memory); 
    private:
    bool compare(int i1, int i2);
    void buildTree(int n, vector<queue<Row> > &runs);
    void replacementSelection(vector<queue<Row> > &runs, int idx);
    Row getWinner(vector<queue<Row> > &runs) ;

};

