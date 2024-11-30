#pragma once

#include "defs.h"
#include<bits/stdc++.h>
#define N 10
typedef uint64_t RowCount;
using namespace std;

class Row
{	
public:
	int size;
	vector<int> record;
	int ovc = 0;
	Row () ;
	Row(int size);
	virtual ~Row ();
	// ...
private:
	// ...
}; // class Row

struct Page {
	vector<Row> rows; // vector or linkedlist
	int rowCount = 0;
};
struct Memory { 
	static vector<Page> buffer; // vector or linkedlist
};

class Plan
{
	friend class Iterator;
public:
	Plan (char const * const name);
	virtual ~Plan ();
	virtual class Iterator * init () const = 0;
protected:
	char const * const _name;
private:
}; // class Plan

class Iterator
{
public:
	Iterator ();
	virtual ~Iterator ();
	void run ();
	virtual bool next (Row & row) = 0;
	virtual void free (Row & row) = 0;
private:
	RowCount _rows;
}; // class Iterator
