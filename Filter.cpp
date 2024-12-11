#include "Filter.h"
#include<bits/stdc++.h>
#include<iostream>
FilterPlan::FilterPlan (char const * const name, Plan * const input)
	: Plan (name), _input (input) // scanPlan
{
	TRACE (true);
} // FilterPlan::FilterPlan

FilterPlan::~FilterPlan ()
{
	TRACE (true);
	delete _input;
} // FilterPlan::~FilterPlan

Iterator * FilterPlan::init () const
{
	TRACE (true);
	return new FilterIterator (this);
} // FilterPlan::init

FilterIterator::FilterIterator (FilterPlan const * const plan) :
	_plan (plan), _input (plan->_input->init ()),
	_consumed (0), _produced (0)
{
	TRACE (true);
} // FilterIterator::FilterIterator

FilterIterator::~FilterIterator ()
{
	TRACE (true);

	delete _input;

	
	traceprintf ("FILTER produced %lu of %lu rows\n",
			(unsigned long) (_produced),
			(unsigned long) (_consumed));
} // FilterIterator::~FilterIterator

bool FilterIterator::next (Row & row)
{
	TRACE (true);

	for (;;)
	{
		if ( ! _input->next (row))  return false;

		++ _consumed;
		/* 
			Following filter gives us the outliers
			This is a range-based filtering. Input data is in the range of 0-100
			User is interested in values which are too less or too high
			i.e first column value < 30 or > 70
		*/
		if(row.record[0] < 30 || row.record[0] > 70) 
			break;
		_input->free (row);
	}

	++ _produced;
	return true;
} // FilterIterator::next

void FilterIterator::free (Row & row)
{
	TRACE (true);
	_input->free (row);
} // FilterIterator::free


/*
filter iteraters over the rows until a valid row is found and then breaks, else
keeps looping

consumed++ everytime
produced++ only when a valid row is found then return true;

*/



