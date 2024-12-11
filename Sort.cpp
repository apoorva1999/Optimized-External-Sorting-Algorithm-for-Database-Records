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

int getInitialRunsToMerge() {
	return (InternalSort::runNumber-2)%(FAN_IN-1)+2;
}

int pidx =0 ;
void getSmallestRunsMetadataToMerge(int totalRunstoMerge)
{
	int cnt=0;
    while (cnt < totalRunstoMerge && SortPlan::runPriority.size())
    {
        RunMetadata runMetadata = SortPlan::runPriority.top();
        SortPlan::runPriority.pop();
        SortPlan::runsToMergeMetadata.push_back(runMetadata);
        cnt++;
    }
}


SortPlan::SortPlan (char const * const name, Plan * const input)
	: Plan (name), _input (input)
{
	TRACE (true);
} // SortPlan::SortPlanrean

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
	if(!filesystem::exists(SortPlan::pass_0_dirname) && !filesystem::create_directory(SortPlan::pass_0_dirname)) {
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
			if(Memory::buffer.size() == FAN_IN) {
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


////////////////////////////////////////////////////////////////////////////////////

					// EXTERNAL SORT //

////////////////////////////////////////////////////////////////////////////////////
	
	ExternalSort::currentRunsToMerge = InternalSort::runNumber;

	while(ExternalSort::currentRunsToMerge > FAN_IN) {
		int totalRunsToGenerate;
		if(ExternalSort::currentPassNumber == 1) {
			totalRunsToGenerate = 1; 
			int initialRuns = getInitialRunsToMerge();  // Graceful degradation
			getSmallestRunsMetadataToMerge(initialRuns);
		} else {
			totalRunsToGenerate = ExternalSort::currentRunsToMerge/FAN_IN;
			getSmallestRunsMetadataToMerge(FAN_IN);
		}
        		
		while(ExternalSort::currentRunNumber < totalRunsToGenerate) {
				ExternalSort::mergeSortedRuns();
				ExternalSort::currentRunNumber++;
				SortPlan::runsToMergeMetadata = vector<RunMetadata>();
				if(ExternalSort::currentRunNumber<totalRunsToGenerate)
				{
                    getSmallestRunsMetadataToMerge(FAN_IN);
                }	
		}
		ExternalSort::currentRunsToMerge = SortPlan::runPriority.size();
		ExternalSort::currentPassNumber++;
		ExternalSort::currentRunNumber = 0;
	}

	getSmallestRunsMetadataToMerge(FAN_IN);
	ExternalSort::mergeLastRun();

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
	
	row = Tree::getWinner();
	for(auto c: row.record) {
		cout<<c<<" ";
	}
	cout<<endl;
	
	Memory::buffer[0].rows.push_back(row);
	if(Memory::buffer[0].rows.size() == PAGE_SIZE) {
		Disk::flushPage("outputFile", Memory::buffer[0], pidx, 0);
	}
    ++_produced;
	if(_produced == _consumed && Memory::buffer[0].rows.size()>0) {
		Disk::flushPage("outputFile", Memory::buffer[0], pidx, 0);
	}
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
