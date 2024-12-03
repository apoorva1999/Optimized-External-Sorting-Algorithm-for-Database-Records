#include "TournamentTree.h"
#include<bits/stdc++.h>
#include <queue> 
#include<iostream>
#include <bit>
#include"Disk.h"
#include "InternalSort.h"
#include"Sort.h"

int InternalSort::runNumber = 0;
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
    string filename= "run_" + to_string(runNumber);
    string filePath = SortPlan::phase_0_dirname+"/"+filename;
    int pidx = 0;
    int cnt=0;
    while(cnt < m) {
        Row row = Tree::getWinner();
        cnt++;
        outputPage.rows.push_back(row);
        if(outputPage.rows.size() == PAGE_SIZE) {
            Disk::flushPage(filePath, outputPage, pidx);
        }
    }

     if(outputPage.rows.size() >0) {
        Disk::flushPage(filePath, outputPage, pidx);
    }

    runNumber++;
}