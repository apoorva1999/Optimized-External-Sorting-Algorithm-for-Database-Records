#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "Witness.h"
#include "Disk.h"


void cleanup() {
	std::filesystem::path targetDirectory = "./"; // Change to your target directory
    std::string pattern = "pass_";
	Disk::deleteDirectories(targetDirectory,pattern);
}

int main (int argc, char * argv [])
{
	cleanup();
	TRACE (true);

	Plan * const plan =
			new WitnessPlan ("output",
				new SortPlan ("*** The main thing! ***",
					new WitnessPlan ("input",
						new FilterPlan ("half",
							new ScanPlan ("source",100)
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
