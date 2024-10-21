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
	if (_count > _plan->_count)
		return false;

	if (_plan->_file.is_open())
	{
		if (std::getline(_plan->_file, _plan->_currentLine))
		{
			std::string str = _plan->_currentLine;
			std::stringstream ss(str);
			int num;
			std::cout<<"SCAN"<<std::endl;
			while (ss >> num)
			{
				std::cout<<num<<" ";
				row.record.push_back(num);
			}
			std::cout<<std::endl;

			
		}

		++_count;
 // Successfully read a line
	}
	
	// ++_count;
	return true;
} // ScanIterator::next

void ScanIterator::free(Row &row)
{
	row.record = std::vector<int>();
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
