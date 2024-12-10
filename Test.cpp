#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "Witness.h"
#include "Disk.h"




int main (int argc, char * argv [])
{

	TRACE (true);
	if(argc > 1) {
		INPUT_SIZE = stoi(argv[1]);
	}

	Plan * const plan =
			new WitnessPlan ("output",
				new SortPlan ("*** The main thing! ***",
					new WitnessPlan ("input",
						new FilterPlan ("half",
							new ScanPlan ("source",INPUT_SIZE)
						)
					)
				)
			);

	Iterator * const it = plan->init ();
	it->run ();
	delete it;

	delete plan;
// 
	return 0;
} // main
