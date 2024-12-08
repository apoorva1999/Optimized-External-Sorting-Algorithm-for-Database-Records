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
    Row senitnelRow = Row(); 
    senitnelRow.record = sentinel_record;
    int cnt = 0;
    vector<queue<Row>>cacheSizeRuns;
    queue<Row>cacheSizeRun;
    int totalRecords = 0;
    for(auto &page : Memory::buffer) {
        
        for(auto row : page.rows) {
            totalRecords++;
            queue<Row>run;
            run.push(row);
            run.push(senitnelRow); 
            SortPlan::runs.push_back(run);
            cnt++;
            if(cnt == CACHE_SIZE) {
                    Tree::buildTree();
                    while(cacheSizeRun.size() < CACHE_SIZE) { 
                        Row row = Tree::getWinner();
                        cacheSizeRun.push(row);
                    }
                    cacheSizeRun.push(senitnelRow);
                    cacheSizeRuns.push_back(cacheSizeRun);
                    cnt = 0;
                    SortPlan::runs = vector<queue<Row>>();
                    cacheSizeRun = queue<Row>();
            }
        }
    }
    if(cnt > 0) {
        Tree::buildTree();
        while(cacheSizeRun.size() < cnt) 
        { 
            Row row = Tree::getWinner();
            cacheSizeRun.push(row);
        }
        cacheSizeRun.push(senitnelRow);
        cacheSizeRuns.push_back(cacheSizeRun);
        cnt = 0;
    }
    // int totalRecords = 0;
    // SortPlan::runs = vector<queue<Row>>();  
    // for(auto &page : Memory::buffer) {
    //     queue<Row>run;
    //     for(auto row : page.rows) {
    //          run.push(row);
    //          totalRecords++;
    //     }CACHE_SIZE
    SortPlan::runs = cacheSizeRuns;
    int totalPages = (int)ceil((double)totalRecords/PAGE_SIZE);
    Tree::buildTree();
    Page outputPage; // take it from memory
    string filename= "run_" + to_string(runNumber);
    string filePath = SortPlan::pass_0_dirname+"/"+filename;
    int pidx = 0;
    cnt=0;
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