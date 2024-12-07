#include "Sort.h"
#include "defs.h"
#include "Iterator.h"
#include "TournamentTree.h"
#include <bit>
#include "InternalSort.h"
#include<iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Disk.h"
#include <filesystem>
#include "ExternalSort.h"

vector<queue<Row>> SortPlan::runs;
string SortPlan::pass_0_dirname = "pass_0";
priority_queue<RunMetadata> SortPlan::runPriority;
vector<RunMetadata> SortPlan::runsToMergeMetadata = vector<RunMetadata>();
int initialRunsToMerge() {
	return (InternalSort::runNumber-2)%(FAN_IN-1)+2;
}
SortPlan::SortPlan (char const * const name, Plan * const input)
	: Plan (name), _input (input)
{
	TRACE (true);
} // SortPlan::SortPlan

SortPlan::~SortPlan ()
{
	TRACE (true);
	delete _input;
} // SortPlan::~SortPlan

Iterator * SortPlan::init () const
{
	TRACE (true);
	return new SortIterator (this);
} // SortPlan::init

SortIterator::SortIterator (SortPlan const * const plan) :
	_plan (plan), _input (plan->_input->init ()),	
	_consumed (0), _produced (0), _currentPageIndex(-1), _currentRowIndex(0)
{
	Tree tree;
	TRACE (true);
// SORT
	Page page;
	if (filesystem::exists(SortPlan::pass_0_dirname)) {
        std::cout << "Directory already exists: " << SortPlan:: pass_0_dirname << std::endl;
    } else if(!filesystem::create_directory(SortPlan::pass_0_dirname)) {
		cerr<<"Couldn't create directory "<<SortPlan::pass_0_dirname<<endl;
		return;//TODO: handle
	}
	for (Row row;  _input->next (row);  _input->free (row))	{
		++ _consumed;
		page.rowCount ++;
		page.rows.push_back(row);
		if(page.rowCount == PAGE_SIZE) {
			TRACE(true);
			Memory::buffer.push_back(page);
			page.rowCount = 0;
			page.rows = vector<Row>();
			if(Memory::buffer.size() == MEMORY_SIZE-1) {
				InternalSort::generateRuns();
				// make initial runs
				/*
					P page size
					M memory size
					(M-1)*P runs of 1 record
					gotta merge them to make a sorted run of (M-1)*P records	
					TournamentTree.sort(memory) //stores initial sorted run of (M-1)*P records in disk
					clear the memory
				*/
				Memory::buffer = vector<Page>();
			}
		}
	}
	// // When rows are not multiple of pageSize or memorySize
	if(page.rows.size()>0) {
		Memory::buffer.push_back(page);
	}
	if(Memory::buffer.size()>0) {
		InternalSort::generateRuns();
	}

	///////////////
	int initialRuns = initialRunsToMerge();


	int cnt = 0;
	
	while(cnt<initialRuns) {
		RunMetadata runMetadata = SortPlan::runPriority.top();
		SortPlan::runPriority.pop();
		SortPlan::runsToMergeMetadata.push_back(runMetadata);
		cnt++;
	}
	
		
	ExternalSort::currentRunsToMerge = InternalSort::runNumber;

	while(ExternalSort::currentRunsToMerge > 1) {
		int totalRunsToGenerate;
		if(ExternalSort::currentPassNumber == 1) {
			totalRunsToGenerate = 1; // Graceful degradation
		} else {
			totalRunsToGenerate = ExternalSort::currentRunsToMerge/FAN_IN;
			while(cnt<FAN_IN && SortPlan::runPriority.size()) {
				RunMetadata runMetadata = SortPlan::runPriority.top();
				SortPlan::runPriority.pop();
				SortPlan::runsToMergeMetadata.push_back(runMetadata);
				cnt++;
			}
		}
        		
		while(ExternalSort::currentRunNumber < totalRunsToGenerate) {
				ExternalSort::mergeSortedRuns(SortPlan::runsToMergeMetadata);
				ExternalSort::currentRunNumber++;
				cnt=0;
				SortPlan::runsToMergeMetadata = vector<RunMetadata>();
				
				if(ExternalSort::currentRunNumber<totalRunsToGenerate)
				{
					while(cnt<FAN_IN && SortPlan::runPriority.size()) {
						RunMetadata runMetadata = SortPlan::runPriority.top();
						SortPlan::runPriority.pop();
						SortPlan::runsToMergeMetadata.push_back(runMetadata);
						cnt++;
				}}
				
		}
		ExternalSort::currentRunsToMerge = SortPlan::runPriority.size();
		ExternalSort::currentPassNumber++;
		ExternalSort::oldRunNumber = 0;
		ExternalSort::currentRunNumber = 0;
	}


	delete _input;

	traceprintf ("%s consumed %lu rows\n",
			_plan->_name,
			(unsigned long) (_consumed));
} // SortIterator::SortIterator


SortIterator::~SortIterator ()
{
	TRACE (true);

	traceprintf ("%s produced %lu of %lu rows\n",
			_plan->_name,
			(unsigned long) (_produced),

			(unsigned long) (_consumed));
} // SortIterator::~SortIterator


bool SortIterator::next(Row &row) {
    TRACE(true);

    if (_produced >= _consumed) {
        return false;
    }

    // if (_currentPageIndex == -1 || _currentRowIndex >= _currentPage.rows.size()) {
    //     ++_currentPageIndex;
    //     _currentPage = Disk::readPage("initial_runs", _currentPageIndex);
    //     _currentRowIndex = 0;
    //     if (_currentPage.rows.empty()) {
    //         return false;
    //     }
    // }

    // row = _currentPage.rows[_currentRowIndex];
    // ++_currentRowIndex;
    ++_produced;
    return true;
}

void SortIterator::free (Row & row)
{
	TRACE (true);
} // SortIterator::free


// empid, age, salary, etc...

/*

Ram to Cache


*/
