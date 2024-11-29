#include "TournamentTree.h"
#include<bits/stdc++.h>
#include <queue> 
#include<iostream>
#include <bit>


using namespace std;

vector<queue<Row>>runs;

int offset(int ovc) {
    return ovc>>24;
}

int value(int ovc) {
    return (ovc&0xFFFFFF);
}

    //OVC and compare
bool Tree::compare(int idx1, int idx2) {
        
        Row& row1 = runs[idx1].front();
        Row& row2 = runs[idx2].front();
        int n = row1.record.size();
        int col = 0;
        // cout<<"idx1 "<<idx1<<" idx2 "<<idx2<<endl;
        if(row1.ovc!=0 && row2.ovc!=0) {
            cout<<"row1: ovc-> "<<row1.ovc<<" off->"<<offset(row1.ovc)<<" val->"<<value(row1.ovc)<<endl;
            cout<<"row2: ovc-> "<<row2.ovc<<" off->"<<offset(row2.ovc)<<" val->"<<value(row2.ovc)<<endl;   

            if(offset(row1.ovc) < offset(row2.ovc)) {
                return false; // row1 has smaller offset than row2, therefore its bigger and loser
            }
            else if(offset(row1.ovc) > offset(row2.ovc)) {
                return true;
            }
            else { //both have same offset
                if(value(row1.ovc) < value(row2.ovc)) {
                    return true; // row1 has smaller value than row2 -> row1 is winner
                } 
                else if(value(row1.ovc) > value(row2.ovc)) {
                    return false;
                }
                else{
                    //both have same offset and value
                    col = offset(row1.ovc)+1;
                }
            }
        }

        for(int i=col;i<n;i++) {
            if(row1.record[i]<row2.record[i]) {
                // cout<<"i "<<i<<" row2.record[i] "<<row2.record[i]<<endl;
                int ovc = (i<<24) | (row2.record[i] & 0xFFFFFF); //storing offest in first 8 bits and value at last 24 bits
                row2.ovc = ovc;
                // cout<<"ovc for row2 "<<row2.ovc<<" at i "<<i<< endl;
                return true;
            } else if(row1.record[i]>row2.record[i]) {
                int ovc = (i<<24) | (row1.record[i] & 0xFFFFFF);
                row1.ovc = ovc;
                // cout<<"ovc for row1 "<<ovc<<" at i "<<i<< endl;
                return false;
            }
        }
        row2.ovc = n<<24;

                // cout<<"ovc for row2 "<< row2.ovc<<" at i "<<n<< endl;

        return true;
}
    
void Tree::buildTree(int n, vector<queue<Row> > &runs) {
        tree = vector<Node>(n);
        int leafn = n/2; //leafs is number of rows/2
        for(int i=0;i<leafn;i++) {
            int j = i+leafn;
            int lefti = 2*i;
            int righti = 2*i+1;
            if(compare(lefti, righti)) {
                    tree[j].lid = righti;
                    tree[j].wid = lefti;
            } 
            else {
                tree[j].lid = lefti;
                tree[j].wid = righti;
            }
            
        }
        for(int i=leafn-1;i>0;i--) {
            int left_child = 2*i;
            int right_child = 2*i+1;
            // cout<<runs[tree[left_child].wid].front().record[0]<<" "<<runs[tree[right_child].wid].front().record[0]<<endl;
            if(compare(tree[left_child].wid, tree[right_child].wid)) {
                tree[i].lid = tree[right_child].wid;
                tree[i].wid = tree[left_child].wid;
            } else {
                tree[i].lid = tree[left_child].wid;
                tree[i].wid = tree[right_child].wid;
            }
        }

        // for(int i=1;i<n;i++) {
        //     cout<<"i: "<<i<<": ";
        //     cout<<"lid: "<<tree[i].lid<<", ";
        //     cout<<"wid: "<<tree[i].wid<<endl;
        // }
}

void Tree::replacementSelection(vector<queue<Row> > &runs, int idx) {
    int pidx = idx/2; // parent index
    int leafn = runs.size()/2;
    int lidx = leafn + pidx; // leaf index
    // cout<<"lidx: "<<lidx<<" idx: "<<idx<<endl;
    if(compare(idx, tree[lidx].lid)) {
        tree[lidx].wid = idx;
        tree[lidx].lid = tree[lidx].lid;
    } else {
        tree[lidx].wid = tree[lidx].lid;
        tree[lidx].lid = idx;
    }

    while(lidx > 1 ) {
        pidx = lidx/2;
        if(compare(tree[lidx].wid, tree[pidx].lid)) {
            tree[pidx].wid = tree[lidx].wid;
            tree[pidx].lid = tree[pidx].lid;
        } else {
         tree[pidx].wid = tree[pidx].lid;
         tree[pidx].lid = tree[lidx].wid;
        }
        lidx = pidx;
    }

    // cout<<"**********"<<endl;
    //         for(int i=1;i<runs.size();i++) {
    //         cout<<"i: "<<i<<": ";
    //         cout<<"lid: "<<tree[i].lid<<", ";
    //         cout<<"wid: "<<tree[i].wid<<endl;
    //     }
    // cout<<"**********"<<endl;
}

Row Tree::getWinner(vector<queue<Row> > &runs) {
    int winnerIdx = tree[1].wid;
    Row row = runs[winnerIdx].front();
    runs[winnerIdx].pop();
    replacementSelection(runs, winnerIdx);
    return row;
}

void Tree:: generateRuns(Memory &memory) {
        TRACE(true);
        vector<int> sentinel_record(1, INT_MAX);
        Row senitnelRow = Row(1); 
        senitnelRow.record = sentinel_record;
        for(auto page : memory.buffer) {
            for(auto row : page.rows) {
                queue<Row>run;
                for(auto r: row.record) {
                    cout<<r<<" ";
                }
                cout<<endl;
                run.push(row);
                run.push(senitnelRow); 
                runs.push_back(run);
            }
        }
        int m = runs.size();
        int n = std::bit_ceil(runs.size()); //make #runs as power of 2

        while(runs.size()<n) {
            queue<Row>run;
            run.push(senitnelRow); 
            runs.push_back(run);
        }

        buildTree(n, runs);

        vector<Row>sortedRun;
        while(sortedRun.size() < m) {
            sortedRun.push_back(getWinner(runs));
        }
        cout<<"******************"<<endl;
        for(auto row:sortedRun) {
            for(auto col: row.record) {
                cout<<col<<", ";
            }
            cout<<endl;
        }
}


