#include "Witness.h"

WitnessPlan::WitnessPlan (char const * const name, Plan * const input)
	: Plan (name), _input (input)
{
	TRACE (true);
} // WitnessPlan::WitnessPlan

WitnessPlan::~WitnessPlan ()
{
	TRACE (true);
	delete _input;
} // WitnessPlan::~WitnessPlan

Iterator * WitnessPlan::init () const
{
	TRACE (true);
	return new WitnessIterator (this);
} // WitnessPlan::init

WitnessIterator::WitnessIterator (WitnessPlan const * const plan) :
	_plan (plan), _input (plan->_input->init ()),
	_rows (0), _xor(0), _inversions(0), _prevRow()
{
	TRACE (true);
} // WitnessIterator::WitnessIterator

WitnessIterator::~WitnessIterator ()
{
	TRACE (true);

	delete _input;

	traceprintf ("%s witnessed TOTAL_ROWS = %lu\n ",
			_plan->_name,
			(unsigned long) (_rows));
	traceprintf ("%s witnessed XOR = %lu\n ",
			_plan->_name,
			(unsigned long)(_xor));
	traceprintf ("%s witnessed INVERSIONS = %lu\n ",
		_plan->_name,
		(unsigned long)(_inversions));
} // WitnessIterator::~WitnessIterator

bool WitnessIterator::next (Row & row)
{
	TRACE (true);

	if ( ! _input->next (row))  return false;
	
	++ _rows;
	for(auto col:row.record) {
		_xor^=col;
	}
	if(_prevRow.record.empty())
	 _prevRow = row;

	else {
		for(int i=0;i<ROW_SIZE;i++) {
			if(row.record[i] < _prevRow.record[i]) {
				_inversions++;
				break;
			} else if(row.record[i] > _prevRow.record[i]) {
				break;
			}
		}
		_prevRow = row;
	}
	return true;
} // WitnessIterator::next

void WitnessIterator::free (Row & row)
{
	TRACE (true);
	_input->free (row);
} // WitnessIterator::free
