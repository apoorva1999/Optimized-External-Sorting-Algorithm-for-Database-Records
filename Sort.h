#include "Iterator.h"
#include<queue>

class RunMetadata {
	public:
	int totalPages;
	int runNumber;
	int passNumber;

	bool operator<(const RunMetadata& other) const {
        return totalPages > other.totalPages;
    }
};
class SortPlan : public Plan
{
	friend class SortIterator;
public:
	SortPlan (char const * const name, Plan * const input);
	~SortPlan ();
	Iterator * init () const;
	static vector<queue<Row>>runs;
	static string pass_0_dirname;
	static priority_queue<RunMetadata> runPriority;
	static vector<RunMetadata>runsToMergeMetadata;
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
private:
	SortPlan const * const _plan;
	Iterator * const _input;
	RowCount _consumed, _produced;
	int _currentPageIndex;
	int _currentRowIndex;
	Page _currentPage;
}; // class SortIterator
