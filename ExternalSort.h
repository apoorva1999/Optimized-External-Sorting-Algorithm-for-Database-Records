#include<bits/stdc++.h>

using namespace std;

class ExternalSort {
    public:
    static int currentPassNumber;
    static int oldRunNumber;
    static int currentRunNumber;
    static int totalRunsToMerge;
    static vector<int> inputPageIdx;
    static int currentRecords;
    static int totalRecords;
    static void mergeSortedRuns();
};