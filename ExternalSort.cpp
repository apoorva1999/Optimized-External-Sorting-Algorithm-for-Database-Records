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
int ExternalSort::currentRunNumber = 0;
int ExternalSort::currentRecords = 0;
int ExternalSort::totalRecords = 0;
int ExternalSort::currentRunsToMerge = InternalSort::runNumber;
vector<int> ExternalSort::inputPageIdx = vector<int>(FAN_IN,0);

// Constructs a file path for a specific run in a given pass
// Example: pass_1/run_0
string getFilePath(int passNumber, int runNumber) {
    return "pass_" + to_string(passNumber) + "/" + "run_" + to_string(runNumber);
}

// Loads the first page of each run to be merged into memory buffers
void fillMemoryWithRuns() {
    Memory::buffer = vector<Page>(MEMORY_SIZE);
    for(int i=0;i<SortPlan::runsToMergeMetadata.size();i++) {
        RunMetadata runMetadata = SortPlan::runsToMergeMetadata[i];
        int passNumber = runMetadata.passNumber;
        int runNumber = runMetadata.runNumber;

        string inputFilePath = getFilePath(passNumber, runNumber);
        if(filesystem::exists(inputFilePath)) {
            Page page = Disk::readPage(inputFilePath, 0); //check that file exist
             Memory::buffer[i] = page;
        }
    }
}

// Converts the pages in memory into runs
void createRunsFromMemory() {
    SortPlan::runs = vector<queue<Row>>();
    ExternalSort::totalRecords = 0;
    ExternalSort::currentRecords = 0;
    for(auto page : Memory::buffer) {
        queue<Row>run;
        for(auto row : page.rows) { 
            run.push(row);
            ExternalSort::totalRecords++;
        }
        if(run.size())
        SortPlan::runs.push_back(run);
    }
}

// Merges sorted runs into a single output run using tournament tree
void ExternalSort::mergeSortedRuns() {
    
    fillMemoryWithRuns();
    createRunsFromMemory();
    Tree::buildTree();

    Page outputPage;

    string outputDir = "pass_" + to_string(currentPassNumber);
    if(!filesystem::exists(outputDir) && !filesystem::create_directory(outputDir)) {
		cout<<"Couldn't create directory "<<outputDir<<endl;
		return;
	}
    string outputFilePath = outputDir + "/run_" + to_string(currentRunNumber);

    int pidx = 0;
    while(currentRecords<totalRecords) {
        Row row = Tree::getWinner();
        currentRecords++;
        outputPage.rows.push_back(row);
        if(outputPage.rows.size() == PAGE_SIZE) {
            Disk::flushPage(outputFilePath, outputPage, pidx, 1);
        }
    }

    if(outputPage.rows.size()) {
        Disk::flushPage(outputFilePath, outputPage, pidx, 1);
    }

    int numberOfPagesInCurrentRun = (int)ceil((double)totalRecords/PAGE_SIZE);
    SortPlan::runPriority.push({numberOfPagesInCurrentRun, currentRunNumber, currentPassNumber});
}

// Builds the tournament tree for merging the last run
void ExternalSort::mergeLastRun() {

    fillMemoryWithRuns();
    createRunsFromMemory();
    Tree::buildTree();
    Memory::buffer = vector<Page>(MEMORY_SIZE);

}