# External Sorting for Large Databases


## Summary

Developed an optimized External Sort algorithm to efficiently sort database records utilizing the underlying storage
hierarchy by implementing cache-size mini-runs, RAM-to-disk spilling, and graceful degradation. Implemented k-way External Merge Sort algorithm in C++ using a Tree-of-Losers tournament data structure and
Offset-Value Coding (OVC) to minimize column-value comparisons, optimizing merge patterns and page sizes

## Testing

We've created a test script to efficiently test our program on different test cases. There exists a specific `tests/`directory which holds the testcases. The testing framework just starts at `1` and keeps incrementing
tests until it can't find any more or encounters a failure. Thus, adding new
tests is easy; just add the relevant files to the tests directory at the lowest available number.

The files needed to describe a test number `n` are:
- `n.run`: How to run the test (which arguments it needs, etc.)
- `n.desc`(optional): A short text description of the test
- `n.pre`: Script to clean up files generated in the previous tests


### Steps to Run the Test Script

- cd tests
- Run `run-tests.sh` command to run all the tests


The options for `run-tests.sh` include:

- `-v` (verbose: print what each test is doing on the console)
- `-t n` (run only test `n`)

