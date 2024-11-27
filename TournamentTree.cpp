#include "TournamentTree.h"
#include<bits/stdc++.h>
#include <queue> 
#include<iostream>
#include <bit>


using namespace std;


    //OVC and compare
bool Tree::compare(queue<Row>&r1, queue<Row>&r2) {
        Row row1 = r1.front();
        Row row2 = r2.front();
        int n = row1.record.size();

        for(int i=0;i<n;i++) {
            // cout<<"row1.record[i] "<<row1.record[i]<<endl;
                // cout<<"row2.record[i] "<<row2.record[i]<<endl;
            if(row1.record[i]<row2.record[i]) {
                int ovc = (i<<24) | (row2.record[i] & 0xFFFFFF); //storing offest in first 8 bits and value at last 24 bits
                row2.ovc = ovc;
                // cout<<"ovc for row2 "<<ovc<<" at i "<<i<< endl;
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
    
void Tree::buildTree(int n, vector<queue<Row> > &data) {
        tree = vector<Node>(n);
        int leafn = n/2; //leafs is number of rows/2
        for(int i=0;i<leafn;i++) {
            int j = i+leafn;
            int lefti = 2*i;
            int righti = 2*i+1;
            if(compare(data[lefti], data[righti])) {
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
            cout<<data[tree[left_child].wid].front().record[0]<<" "<<data[tree[right_child].wid].front().record[0]<<endl;

            if(compare(data[tree[left_child].wid], data[tree[right_child].wid])) {
                tree[i].lid = tree[right_child].wid;
                tree[i].wid = tree[left_child].wid;
            } else {
                tree[i].lid = tree[left_child].wid;
                tree[i].wid = tree[right_child].wid;
            }
        }

        for(int i=1;i<n;i++) {
            cout<<"i: "<<i<<": ";
            cout<<"lid: "<<tree[i].lid<<", ";
            cout<<"wid: "<<tree[i].wid<<endl;
        }
}

void Tree::replacementSelection(vector<queue<Row> > &data, int idx) {
    int j=0;
    
}

Row Tree::getWinner(vector<queue<Row> > &data) {
    int winnerIdx = tree[1].wid
    Row row = data[winnerIdx].front();
    data[winnerIdx].pop();
    replacementSelection(data, winnerIdx);
    return row;
}

void Tree:: generateRuns(Memory &memory) {
        TRACE(true);
        vector<int> sentinel_record(1, INT_MAX);
        Row senitel_row = Row(1); 
        senitel_row.record = sentinel_record;
        vector<queue<Row> > data;
        for(auto page : memory.buffer) {
            for(auto row : page.rows) {
                queue<Row>run;
                cout<<"row-> ";
                for(auto r: row.record) {
                    cout<<r<<" ";
                }
                cout<<endl;
                run.push(row);
                run.push(senitel_row); 
                data.push_back(run);
            }
        }
        int m = data.size();
        int n = std::bit_ceil(data.size());

        while(data.size()<n) {
            queue<Row>run;
            run.push(senitel_row); 
            data.push_back(run);
        }

        buildTree(n, data);

        vector<Row>sortedRun;
        while(sortedRun.size() < m) {
            sortedRun.push_back(getWinner(data));
        }
}


