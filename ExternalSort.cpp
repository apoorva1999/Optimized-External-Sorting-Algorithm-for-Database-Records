// #include "ExternalSort.h"
// #include "defs.h"
// #include "Disk.h"
// #include "Sort.h"
// #include<bits/stdc++.h>
// #include "TournamentTree.h"
// using namespace std;


// vector<int> currentPageIndex;
// int ExternalSort::passNumber = 0;
// void ExternalSort::mergeSortedRuns(string inputFile, int idx) {
//     int runs = MEMORY_SIZE-1;
//     Memory::buffer = vector<Page>(runs);
//     for(int i=0; i<runs; i++) {
//         int pidx = idx+i*(SortIterator::runSize);
//         Page page = Disk::readPage(inputFile, pidx);
//         Memory::buffer[i] = page;
//     }
    
//     vector<int> sentinel_record(1, INT_MAX);
//     Row senitnelRow = Row(1); 
//     senitnelRow.record = sentinel_record;

//     SortPlan::runs = vector<queue<Row>>();
//     int m = 0;
//     for(auto page : Memory::buffer) {
//         queue<Row>run;
//         for(auto row : page.rows) { 
//             run.push(row);
//             m++;
//         }
//         SortPlan::runs.push_back(run);
//     }
//     Tree::buildTree();

//     // int cnt = 0;
//     // Page outputPage;
//     // int runNumber = 1;
//     // while(cnt<m) {
//     //     Row row = Tree::getWinner();
//     //     cnt++;
//     //     outputPage.rows.push_back(row);
//     //     if(outputPage.rows.size() == PAGE_SIZE) {
//     //         Disk::flushPage(outputFile, outputPage);
//     //     }
//     // }
//     // SortIterator::runSize = SortIterator::runSize*(MEMORY_SIZE-1);
//     // passNumber++;
// }