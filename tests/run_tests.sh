#!/bin/bash

# CodeTime Test Runner

echo "Running CodeTime Test Suite..."
echo ""

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counters
TOTAL=0
PASSED=0
FAILED=0

# Function to run a test
run_test() {
    local test_name=$1
    local test_command=$2

    TOTAL=$((TOTAL + 1))
    echo -n "Running: $test_name... "

    if eval "$test_command" > /dev/null 2>&1; then
        echo -e "${GREEN}PASSED${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}FAILED${NC}"
        FAILED=$((FAILED + 1))
    fi
}

# Build the compiler first
echo "Building compiler..."
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo -e "${GREEN}Build successful${NC}"
else
    echo -e "${RED}Build failed${NC}"
    exit 1
fi
echo ""

# Test compiler exists
run_test "Compiler binary exists" "test -f bin/codetime"

# Test compiler help
run_test "Compiler help command" "bin/codetime help"

# Test compiler version
run_test "Compiler version command" "bin/codetime version"

# Test basic hello world
run_test "Hello world compilation" "bin/codetime check examples/hello.cdt"

# Test variables example
run_test "Variables example" "bin/codetime check examples/variables.cdt"

# Test functions example
run_test "Functions example" "bin/codetime check examples/functions.cdt"

# Test conditions example
run_test "Conditions example" "bin/codetime check examples/conditions.cdt"

# Test loops example
run_test "Loops example" "bin/codetime check examples/loops.cdt"

# Test collections example
run_test "Collections example" "bin/codetime check examples/collections.cdt"

# Test objects example
run_test "Objects example" "bin/codetime check examples/objects.cdt"

# Test patterns example
run_test "Patterns example" "bin/codetime check examples/patterns.cdt"

# Test comprehensive example
run_test "Comprehensive example" "bin/codetime check examples/comprehensive.cdt"

# Test invalid syntax (should fail)
echo ""
echo "Testing error handling..."
run_test "Invalid syntax detection" "! bin/codetime check /dev/null"

# Summary
echo ""
echo "=========================================="
echo "Test Summary"
echo "=========================================="
echo -e "Total:   $TOTAL"
echo -e "${GREEN}Passed:  $PASSED${NC}"
echo -e "${RED}Failed:  $FAILED${NC}"
echo "=========================================="

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed${NC}"
    exit 1
fi
