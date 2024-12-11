# External Sorting for Large Databases

## Group members
- Student ID-1: 9086615722
- Student ID-2: 9087239522

## Overview 
Implemented an optimized External Sort algorithm to efficiently sort database records utilizing the underlying storage hierarchy by implementing cache-size mini-runs, RAM-to-disk spilling, and graceful degradation. Implemented k-way External Merge Sort algorithm in C++ using a Tree-of-Losers tournament data structure and Offset-Value Coding (OVC) to minimize column-value comparisons, optimizing merge patterns and page sizes

## Description

### Techniques Implemented by our submission and the corresponding Source Files

- **Tournament trees**: `File TournamentTree.cpp`
- **Offset-value coding**: `File TournamentTree.cpp`
- **Minimum count of row & column comparisons**
- **Cache-size mini runs**: `File InternalSort.cpp`
- **External sort**: `File ExternalSort.cpp`
- **Graceful degradation**: `File Sort.cpp`
  - **Into merging** 
  - **Beyond one merge step**
- **Optimized merge patterns**: `File Sort.cpp`
- **Verifying**: 
  - **sets of rows & values**: `File Witness.cpp`
  - **inversions**: `File Witness.cpp`

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

- The options for `run-tests.sh` include:

    - `-v` (verbose: print what each test is doing on the console)
    - `-t n` (run only test `n`)

After running the tests, there are some output files generate which help in understanding result of each test

- Following files are generated in the `tests/tests-out` directory. 
    -  `n.out`: contains all the logs from each test
    - `n.err`: contains error message if any
    - `n.rc` : contains return code of the test 
- Following files are generated in the `tests` directory
    - `pass_*/run_*` : When tests are run individually, you can see the passes and runs generated for that test including ovc
    - `outputFile` : When tests are run individually, this file stores the sorted records without ovc


