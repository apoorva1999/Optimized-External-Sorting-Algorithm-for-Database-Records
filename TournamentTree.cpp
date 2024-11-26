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
            if(row1.record[i]<row2.record[i]) {
                int ovc = i;  //offset value coding
                ovc*=10;
                ovc+=row2.record[i];
                row2.ovc = ovc;
                return true;
            } else if(row1.record[i]>row2.record[i]) {
                int ovc = i;
                ovc*=10;
                ovc+=row1.record[i]; //what happens if offset is two digit/three digit
                row1.ovc = ovc;
                return false;
            }
        }
        row2.ovc = n*10;
        return true;
}
    
void Tree::buildTree(int n, vector<queue<Row> > &data) {
        vector<Node>tree(n);
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
            if(compare(data[tree[left_child].wid], data[tree[right_child].wid])) {
                tree[i].lid = tree[right_child].wid;
                tree[i].wid = tree[left_child].wid;
            } else {
                tree[i].lid = tree[left_child].wid;
                tree[i].wid = tree[right_child].wid;
            }
        }
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
                run.push(row);
                cout<<"--------"<<endl;
                for(auto col: row.record) {
                    cout<<col<<" ";
                }
                cout<<endl;
                cout<<"--------"<<endl;

                run.push(senitel_row); 
                data.push_back(run);
            }
        }

        int n = std::bit_ceil(data.size());
        while(data.size()<n) {
            queue<Row>run;
            run.push(senitel_row); 
            data.push_back(run);
        }
        buildTree(n, data);
}


