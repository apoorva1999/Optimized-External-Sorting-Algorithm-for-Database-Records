#include "Scan.h"
#include <iostream>
#include <fstream>
#include <sstream>

ScanPlan::ScanPlan(char const *const name, RowCount const count)
	: Plan(name), _count(count), _file("table.txt")
{

	// std::ifstream file("table.txt");
	TRACE(true);
} // ScanPlan::ScanPlan

ScanPlan::~ScanPlan()
{
	TRACE(true);
} // ScanPlan::~ScanPlan

Iterator *ScanPlan::init() const
{
	TRACE(true);
	return new ScanIterator(this);
} // ScanPlan::init

ScanIterator::ScanIterator(ScanPlan const *const plan) : _plan(plan), _count(0)
{
	TRACE(true);
} // ScanIterator::ScanIterator

ScanIterator::~ScanIterator()
{
	TRACE(true);
	traceprintf("produced %lu of %lu rows\n",
				(unsigned long)(_count),
				(unsigned long)(_plan->_count));
} // ScanIterator::~ScanIterator

bool ScanIterator::next(Row &row)
{
	TRACE(true);

	if (_plan->_file.is_open())
	{
		if (std::getline(_plan->_file, _plan->_currentLine))
		{
			std::string str = _plan->_currentLine;
			std::stringstream ss(str);
			int num;
			while (ss >> num)
			{
				row.record.push_back(num);
			}
		}

		++_count;
		return true; // Successfully read a line
	}
	if (_count >= _plan->_count)
		return false;

	++_count;
	return true;
} // ScanIterator::next

void ScanIterator::free(Row &row)
{
	TRACE(true);
} // ScanIterator::free

/*
ScanPlan
	- name
	- count

	- init
		- new ScanIterator(plan)

	- ScanIterator
		- plan
		- rowcount
		- next
		- free

*/
