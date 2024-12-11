#! /usr/bin/env bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NONE='\033[0m'
CYAN='\033[1;36m'
# run_test testdir testnumber
run_test () {
    local testdir=$1
    local testnum=$2
    local verbose=$3

    # pre: execute this after before the test is done, to set up
    local prefile=$testdir/test.pre
    if [[ -f $prefile ]]; then
	eval $(cat $prefile)
	if (( $verbose == 1 )); then
	    echo -n "pre-test:  "
	    cat $prefile
	fi
    fi
    local testfile=$testdir/$testnum.run
    if (( $verbose == 1 )); then
	echo -e "test:      "
	cat $testfile
    fi
    if (( verbose == 1 )); then
    # Print to console and save to files
    eval $(cat "$testfile") 2> "tests-out/$testnum.err" | tee "tests-out/$testnum.out"
    else
        # Save to files only
        eval $(cat "$testfile") > "tests-out/$testnum.out" 2> "tests-out/$testnum.err"
    fi

    echo $? > tests-out/$testnum.rc

    # post: execute this after the test is done, to clean up
    local postfile=$testdir/$testnum.post
    if [[ -f $postfile ]]; then
	eval $(cat $postfile)
	if (( $verbose == 1 )); then
	    echo -e "post-test: "
	    cat $postfile
	fi
    fi
    return 
}

print_error_message () {
    local testnum=$1
    local contrunning=$2
    local filetype=$3
    builtin echo -e "test $testnum: ${RED}$testnum.$filetype incorrect${NONE}"
    echo "  what results should be found in file: $testdir/$testnum.$filetype"
    echo "  what results produced by your program: tests-out/$testnum.$filetype"
    echo "  compare the two using diff, cmp, or related tools to debug, e.g.:"
    echo "  prompt> diff $testdir/$testnum.$filetype tests-out/$testnum.$filetype"
    echo "  See tests/$testnum.run for what is being run"
    if (( $contrunning == 0 )); then
	exit 1
    fi
}

# check_test testdir testnumber contrunning out/err
check_test () {
    local testdir=$1
    local testnum=$2
    local contrunning=$3
    local filetype=$4

    # option to use cmp instead?
    returnval=$(diff $testdir/$testnum.$filetype tests-out/$testnum.$filetype)
    if (( $? == 0 )); then
	echo 0
    else
	echo 1
    fi
}

# run_and_check testdir testnumber contrunning verbose printerror
#   testnumber: the test to run and check
#   printerrer: if 1, print an error if test does not exist
run_and_check () {
    local testdir=$1
    local testnum=$2
    local contrunning=$3
    local verbose=$4
    local failmode=$5

    if [[ ! -f $testdir/$testnum.run ]]; then
	if (( $failmode == 1 )); then
	    echo "test $testnum does not exist" >&2; exit 1
	fi
	exit 0
    fi
    
	echo -e "running test $testnum: "
    cat $testdir/$testnum.desc  
    run_test $testdir $testnum $verbose

    local output_file="tests-out/$testnum.out"
    local total_rows_before=$(grep "input witnessed TOTAL_ROWS" "$output_file" | awk -F'=' '{print $2}' | xargs)
    local total_rows_after=$(grep "output witnessed TOTAL_ROWS" "$output_file" | awk -F'=' '{print $2}' | xargs)
    local xor_before=$(grep "input witnessed XOR" "$output_file" | awk -F'=' '{print $2}' | xargs)
    local xor_after=$(grep "output witnessed XOR" "$output_file" | awk -F'=' '{print $2}' | xargs)
    local inversions_before=$(grep "input witnessed INVERSIONS" "$output_file" | awk -F'=' '{print $2}' | xargs)

    local inversions_after=$(grep "output witnessed INVERSIONS" "$output_file" | awk -F'=' '{print $2}' | xargs)
    
    local passed=true

    if [[ "$total_rows_before" != "$total_rows_after" ]]; then
        echo -e "Test $testnum: ${RED}FAILED${NONE} - TOTAL_ROWS mismatch (before: $total_rows_before, after: $total_rows_after)"
        passed=false
    fi

    if [[ "$xor_before" != "$xor_after" ]]; then
        echo -e "Test $testnum: ${RED}FAILED${NONE} - XOR mismatch (before: $xor_before, after: $xor_after)"
        passed=false
    fi

    if [[ "$inversions_after" -ne 0 ]]; then
        echo -e "Test $testnum: ${RED}FAILED${NONE} - INVERSIONS not zero (after: $inversions_after)"
        passed=false
    fi

    if $passed; then
        echo -e "${CYAN}Witnessed TOTAL ROWS (before: $total_rows_before, after: $total_rows_after)"
        echo -e "Witnessed XOR (before: $xor_before, after: $xor_after)"
        echo -e "Witnessed INVERSIONS (before: $inversions_before, after: $inversions_after) ${NONE}"
        echo -e "Test $testnum: ${GREEN}PASSED${NONE}"
    else
        # Optional: exit on failure or continue
        if [[ $contrunning -eq 0 ]]; then
            exit 1
        fi
    fi
}

# usage: call when args not parsed, or when help needed
usage () {
    echo "usage: run-tests.sh [-h] [-v] [-t test] [-c] [-s] [-d testdir]"
    echo "  -h                help message"
    echo "  -v                verbose"
    echo "  -t n              run only test n"
    echo "  -c                continue even after failure"
    echo "  -s                skip pre-test initialization"
    echo "  -d testdir        run tests from testdir"
    return 0
}

#
# main program
#
verbose=0
testdir="tests"
contrunning=0
skippre=0
specific=""

args=`getopt hvsct:d: $*`
if [[ $? != 0 ]]; then
    usage; exit 1
fi

set -- $args
for i; do
    case "$i" in
    -h)
	usage
	exit 0
        shift;;
    -v)
        verbose=1
        shift;;
    -c)
        contrunning=1
        shift;;
    -s)
        skippre=1
        shift;;
    -t)
        specific=$2
	shift
	number='^[0-9]+$'
	if ! [[ $specific =~ $number ]]; then
	    usage
	    echo "-t must be followed by a number" >&2; exit 1
	fi
        shift;;
    -d)
        testdir=$2
	shift
        shift;;
    --)
        shift; break;;
    esac
done

# need a test directory; must be named "tests-out"
if [[ ! -d tests-out ]]; then
    mkdir tests-out
fi

# do a one-time setup step
if (( $skippre == 0 )); then
    if [[ -f tests/pre ]]; then
	echo -e "doing one-time pre-test (use -s to suppress)"
	source tests/pre
	if (( $? != 0 )); then
	    echo "pre-test: failed"
	    exit 1
	fi
	echo ""
    fi
fi

# run just one test
if [[ $specific != "" ]]; then
    run_and_check $testdir $specific $contrunning $verbose 1
    exit 0
fi

# run all tests
(( testnum = 1 ))
while true; do
    run_and_check $testdir $testnum $contrunning $verbose 0
    (( testnum = $testnum + 1 ))
done

exit 0