#include "Iterator.h"
#include<queue>
class SortPlan : public Plan
{
	friend class SortIterator;
public:
	SortPlan (char const * const name, Plan * const input);
	~SortPlan ();
	Iterator * init () const;
	static vector<queue<Row>>runs;
private:
	Plan * const _input;
}; // class SortPlan

class SortIterator : public Iterator
{
public:
	SortIterator (SortPlan const * const plan);
	~SortIterator ();
	bool next (Row & row);
	void free (Row & row);
	static int runSize;

private:
	SortPlan const * const _plan;
	Iterator * const _input;
	RowCount _consumed, _produced;
	int _currentPageIndex;
	int _currentRowIndex;
	Page _currentPage;
}; // class SortIterator
