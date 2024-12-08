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
    for(auto &page : Memory::buffer) {
        SortPlan::runs = vector<queue<Row>>();
        for(auto row : page.rows) {
            queue<Row>run;
            run.push(row);
            run.push(senitnelRow); 
            SortPlan::runs.push_back(run);
        }
        //cache size mini runs
        page.rows = vector<Row>();
        int totalRecords = SortPlan::runs.size();

        Tree::buildTree();
        int cnt=0;
        while(cnt < totalRecords) {
            Row row = Tree::getWinner();
            cnt++;
            page.rows.push_back(row);
        }
    }

    int totalRecords = 0;
    SortPlan::runs = vector<queue<Row>>();  
    for(auto &page : Memory::buffer) {
        queue<Row>run;
        for(auto row : page.rows) {
             run.push(row);
             totalRecords++;
        }
        run.push(senitnelRow);
        SortPlan::runs.push_back(run);
    }

    int totalPages = (int)ceil((double)totalRecords/PAGE_SIZE);
    Tree::buildTree();
    Page outputPage; // take it from memory
    string filename= "run_" + to_string(runNumber);
    string filePath = SortPlan::pass_0_dirname+"/"+filename;
    int pidx = 0;
    int cnt=0;
    SortPlan::runPriority.push({totalPages, runNumber, 0});
    while(cnt < totalRecords) {
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