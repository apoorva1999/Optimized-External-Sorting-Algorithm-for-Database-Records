#include "Sort.h"
#include "defs.h"
#include "Iterator.h"
#include "TournamentTree.h"
#include <bit>
#include<iostream>

int pageSize = 5; //one page can hold 5 records
int MemorySize = 4; // 4 buffer pages in memory

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
	_consumed (0), _produced (0)
{
	Tree tree;
	TRACE (true);
// SORT
	Page page;
	Memory memory;
	for (Row row;  _input->next (row);  _input->free (row))	{
	cout<<"memory size "<<memory.pageCount<<endl;
		++ _consumed;
		page.rowCount ++;
		page.rows.push_back(row);
		if(page.rowCount == PAGE_SIZE) {
			TRACE(true);
			memory.buffer.push_back(page);
			memory.pageCount++;
			cout<<"memory.pagecount "<<memory.pageCount<<endl;
			page.rowCount = 0;
			page.rows = vector<Row>();

			if(memory.pageCount == MEMORY_SIZE-1) {
				cout<<"internal"<<endl;
				tree.generateRuns(memory);
				// make initial runs
				/*
					P page size
					M memory size
					(M-1)*P runs of 1 record
					gotta merge them to make a sorted run of (M-1)*P records	
					TournamentTree.sort(memory) //stores initial sorted run of (M-1)*P records in disk
					clear the memory
				*/
			}
		}
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

bool SortIterator::next (Row & row)
{
	TRACE (true);

	if (_produced >= _consumed)  return false;

	++ _produced;
	return true;
} // SortIterator::next

void SortIterator::free (Row & row)
{
	TRACE (true);
} // SortIterator::free


// empid, age, salary, etc...

/*

Ram to Cache


*/
