#include "TournamentTree.h"
#include<bits/stdc++.h>
#include <queue> 
#include<iostream>
#include <bit>
#include"Disk.h"
#include "ExternalSort.h"
#include"Sort.h"

using namespace std;

vector<Node> Tree::tree;

Row sentinelRow;

int offset(int ovc) {
    return ovc>>24;
}

int value(int ovc) {
    return (ovc&0xFFFFFF);
}

//OVC and compare
bool Tree::compare(int idx1, int idx2) {
        
        Row& row1 = SortPlan::runs[idx1].front();
        Row& row2 = SortPlan::runs[idx2].front();
        int n = row1.record.size();
        int col = 0;
        if(row1.ovc!=0 && row2.ovc!=0) {

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
                int ovc = (i<<24) | (row2.record[i] & 0xFFFFFF); //storing offest in first 8 bits and value at last 24 bits
                row2.ovc = ovc;
                return true;
            } else if(row1.record[i]>row2.record[i]) {
                int ovc = (i<<24) | (row1.record[i] & 0xFFFFFF);
                row1.ovc = ovc;
                return false;
            }
        }
        row2.ovc = n<<24;
        return true;
}
    
void Tree::buildTree() {
    int totalRuns = FAN_IN;
    ExternalSort::inputPageIdx = vector<int>(totalRuns, 1);
    vector<int>sentinel_record (1, INT_MAX);
    sentinelRow.record = sentinel_record;
    int n = std::bit_ceil(SortPlan::runs.size()); //make #runs as power of 2
    while(SortPlan::runs.size()<n) {
        queue<Row>run;
        run.push(sentinelRow); 
        SortPlan::runs.push_back(run);
    }   
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
        if(compare(tree[left_child].wid, tree[right_child].wid)) {
            tree[i].lid = tree[right_child].wid;
            tree[i].wid = tree[left_child].wid;
        } else {
            tree[i].lid = tree[left_child].wid;
            tree[i].wid = tree[right_child].wid;
        }
    }
}

void Tree::replacementSelection(int idx) {
    int pidx = idx/2; // parent index
    int leafn = SortPlan::runs.size()/2;
    int lidx = leafn + pidx; // leaf index
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
}

Row Tree::getWinner() {
    int winnerIdx = tree[1].wid;
    Row row = SortPlan::runs[winnerIdx].front();
    SortPlan::runs[winnerIdx].pop();
  
    if(SortPlan::runs[winnerIdx].empty() && row.record[0]!=INT_MAX) {
          int runNumber = SortPlan::runsToMergeMetadata[winnerIdx].runNumber;
          int passNumber = SortPlan::runsToMergeMetadata[winnerIdx].passNumber;
        string filename = "pass_" + to_string(passNumber) + "/run_" + to_string(runNumber);
        int nextPageIdx = ExternalSort::inputPageIdx[winnerIdx];
        ExternalSort::inputPageIdx[winnerIdx]++;
        Page page = Disk::readPage(filename, nextPageIdx);
        queue<Row>run;
        for(auto row : page.rows) { 
            ExternalSort::totalRecords++;
            run.push(row);
        }
        
        if(run.empty())  run.push(sentinelRow);
        SortPlan::runs[winnerIdx] = run;
    }
    replacementSelection(winnerIdx);
    return row;
}
