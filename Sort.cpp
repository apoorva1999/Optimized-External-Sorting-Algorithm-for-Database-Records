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

vector<queue<Row>> SortPlan::runs;
string SortPlan::phase_0_dirname = "phase_0";
int SortIterator::runSize = 0;

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
	if (filesystem::exists(SortPlan::phase_0_dirname)) {
        std::cout << "Directory already exists: " << SortPlan:: phase_0_dirname << std::endl;
    } else if(!filesystem::create_directory(SortPlan::phase_0_dirname)) {
		cerr<<"Couldn't create directory "<<SortPlan::phase_0_dirname<<endl;
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
				cout<<"runNumber: "<<InternalSort::runNumber<<endl;

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
	// When rows are not multiple of pageSize or memorySize
	if(page.rows.size()>0) {
		Memory::buffer.push_back(page);
	}
	if(Memory::buffer.size()>0) {
		cout<<"left?"<<endl;
				cout<<"runNumber: "<<InternalSort::runNumber<<endl;

		InternalSort::generateRuns();

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
