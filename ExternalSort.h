#include<bits/stdc++.h>
// #include"Sort.h"


using namespace std;

class RunMetadata;  
class ExternalSort {
    public:
    static int currentPassNumber;
    static int oldRunNumber;
    static int currentRunNumber;
    static int currentRunsToMerge;
    static vector<int> inputPageIdx;
    static int currentRecords;
    static int totalRecords;
    static void mergeSortedRuns();
    static void mergeSortedRuns(vector<RunMetadata>&runsToMergeMetadata);
};