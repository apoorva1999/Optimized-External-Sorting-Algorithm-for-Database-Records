#include "TournamentTree.h"
#include<bits/stdc++.h>
#include <queue> 
#include<iostream>
#include <bit>
#include"Disk.h"
#include "InternalSort.h"
#include"Sort.h"
void InternalSort::generateRuns() {
    TRACE(true);
    SortPlan::runs = vector<queue<Row>>();
    vector<int> sentinel_record(1, INT_MAX);
    Row senitnelRow = Row(1); 
    senitnelRow.record = sentinel_record;
    for(auto page : Memory::buffer) {
        for(auto row : page.rows) {
            queue<Row>run;
            for(auto r: row.record) {
                cout<<r<<", ";
            }
            cout<<endl;
            run.push(row);
            run.push(senitnelRow); 
            SortPlan::runs.push_back(run);
        }
    }
    int m = SortPlan::runs.size();
    SortIterator::runSize = (m+MEMORY_SIZE)/(MEMORY_SIZE-1);
    Tree::buildTree();
    cout<<"******"<<endl;
    Page outputPage; // take it from memory
    string file = "initial_runs";
    int cnt=0;
    while(cnt < m) {
        Row row = Tree::getWinner();
        cnt++;
        outputPage.rows.push_back(row);
        if(outputPage.rows.size() == PAGE_SIZE) {
            Disk::flushPage(file, outputPage);
        }
    }
}