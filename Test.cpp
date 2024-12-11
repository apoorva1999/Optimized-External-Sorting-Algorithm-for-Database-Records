#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "Witness.h"
#include "Disk.h"



void printConfiguration() {
    std::cout << "----------------------------------------------------------------\n";
    std::cout << " " << std::setw(20) << "Metric	" << " | " << std::setw(20) << "Value" << " \n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << " " << std::setw(20) << "Memory Size	" << " | " << std::setw(20) << MEMORY_SIZE << " Pages" << " \n";
    std::cout << " " << std::setw(20) << "Page Size	" << " | " << std::setw(20) << PAGE_SIZE << " Records" << " \n";
    std::cout << " " << std::setw(20) << "Cache Size	" << " | " << std::setw(20) << CACHE_SIZE << " Records" << " \n";
    std::cout << " " << std::setw(20) << "Input Size	" << " | " << std::setw(20) << INPUT_SIZE << " Records" << " \n";
    std::cout << " " << std::setw(20) << "Number of Columns	" << " | " << std::setw(20) << ROW_SIZE << " Columns" << " \n";
    std::cout << "----------------------------------------------------------------\n";
}

int main (int argc, char * argv [])
{

	TRACE (true);
	if(argc > 1) {
		INPUT_SIZE = stoi(argv[1]);
	}

	printConfiguration();

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
	printConfiguration();

	return 0;
} // main
