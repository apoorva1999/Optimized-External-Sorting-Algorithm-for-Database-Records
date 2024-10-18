#include "Iterator.h"
#include <iostream>
#include <fstream>
#include <string>

class ScanPlan : public Plan
{
	friend class ScanIterator;
public:
	ScanPlan (char const * const name, RowCount const count);
	~ScanPlan ();
	Iterator * init () const;
private:
	RowCount const _count;
	mutable std::ifstream _file;
	mutable std::string _currentLine; 
}; // class ScanPlan

class ScanIterator : public Iterator
{
public:
	ScanIterator (ScanPlan const * const plan);
	~ScanIterator ();
	bool next (Row & row);
	void free (Row & row);
private:
	ScanPlan const * const _plan;
	RowCount _count;
}; // class ScanIterator