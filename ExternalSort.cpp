#include "ExternalSort.h"
#include "InternalSort.h"
#include "defs.h"
#include "Disk.h"
#include "Sort.h"
#include<bits/stdc++.h>
#include "TournamentTree.h"
#include<filesystem>
#include<iostream>
using namespace std;


int ExternalSort::currentPassNumber = 1;
int ExternalSort::oldRunNumber = 0;
int ExternalSort::currentRunNumber = 0;
int ExternalSort::currentRecords = 0;
int ExternalSort::totalRecords = 0;
int ExternalSort::totalRunsToMerge = InternalSort::runNumber;
vector<int> ExternalSort::inputPageIdx = vector<int>(MEMORY_SIZE-1,0);

void ExternalSort::mergeSortedRuns() {
    cout<<"EXTERNAL SORT"<<endl;
    int totalRunNumber = std::min(MEMORY_SIZE-1, totalRunsToMerge - oldRunNumber);
    Memory::buffer = vector<Page>(MEMORY_SIZE);
    int oldPassNumber = currentPassNumber-1;
//    ;
    for(int i=oldRunNumber; i<oldRunNumber + totalRunNumber; i++) {
        // inputFilePath += "run_" + to_string(i);
        string inputFilePath = "pass_" + to_string(oldPassNumber) + "/" +  "run_" + to_string(i);
        if(filesystem::exists(inputFilePath)) {
            Page page = Disk::readPage(inputFilePath, 0); //check that file exist
             Memory::buffer[i-oldRunNumber] = page;
        }
    }

    /// Infinity row
    vector<int> sentinel_record(1, INT_MAX);
    Row senitnelRow = Row(1); 
    senitnelRow.record = sentinel_record;
    //////


    SortPlan::runs = vector<queue<Row>>();
    totalRecords = 0;
    currentRecords = 0;
    for(auto page : Memory::buffer) {
        queue<Row>run;
        for(auto row : page.rows) { 
            run.push(row);
            totalRecords++;
        }
        if(run.size())
        SortPlan::runs.push_back(run);
    }
    Tree::buildTree();

    Page outputPage;

    string outputDir = "pass_" + to_string(currentPassNumber);
    if (filesystem::exists(outputDir)) {
       cout<< "Directory already exists: " << outputDir << std::endl;
    } else if(!filesystem::create_directory(outputDir)) {
		cout<<"Couldn't create directory "<<outputDir<<endl;
		return;//TODO: handle
	}
    string outputFilePath = outputDir + "/run_" + to_string(currentRunNumber);

    int pidx = 0;
    while(currentRecords<totalRecords) {
        Row row = Tree::getWinner();
        currentRecords++;
        outputPage.rows.push_back(row);
        if(outputPage.rows.size() == PAGE_SIZE) {
            Disk::flushPage(outputFilePath, outputPage, pidx);
        }
    }

    if(outputPage.rows.size()) {
        Disk::flushPage(outputFilePath, outputPage, pidx);
    }
    SortIterator::runSize = SortIterator::runSize*(MEMORY_SIZE-1);
    oldRunNumber+=totalRunNumber;

}