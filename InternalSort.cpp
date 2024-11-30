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
    int n = std::bit_ceil(SortPlan::runs.size()); //make #runs as power of 2

    while(SortPlan::runs.size()<n) {
        queue<Row>run;
        run.push(senitnelRow); 
        SortPlan::runs.push_back(run);
    }

    Tree::buildTree(n, SortPlan::runs);
    cout<<"******"<<endl;
    vector<Row>sortedRun;
    // while(sortedRun.size()<m) {
    //      Row row = Tree::getWinner(SortPlan::runs);
    //      for(auto r: row.record) {
    //             cout<<r<<", ";
    //     }
    //     sortedRun.push_back(row);
    //     cout<<endl;
    // }
    Page outputPage;
    string file = "initial_runs";
    int cnt=0;
    while(cnt < m) {
        Row row = Tree::getWinner(SortPlan::runs);
        cnt++;
           for(auto r: row.record) {
                cout<<r<<", ";
            }
            cout<<endl;
        outputPage.rows.push_back(row);
        // cout<<"before outputpage.rows.size "<<outputPage.rows.size()<<endl;
        if(outputPage.rows.size() == PAGE_SIZE) {
            Disk::flushPage(file, outputPage);
        //  cout<<"after outputpage.rows.size "<<outputPage.rows.size()<<endl;
        }
    }
}