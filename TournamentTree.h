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
    bool compare(queue<Row>&r1, queue<Row>&r2);
    void buildTree(int n, vector<queue<Row> > &data);
};

