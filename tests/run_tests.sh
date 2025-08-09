#!/bin/bash

# Test runner script for miniRT unit tests
# This script compiles and runs all unit tests

echo "=== miniRT Unit Test Suite ==="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test results
PASSED=0
FAILED=0

# Function to run a single test
run_test() {
    local test_name=$1
    local test_file=$2
    
    echo -e "${YELLOW}Running $test_name...${NC}"
    
    # Compile the test
    gcc -Wall -Wextra -Werror -I.. -I../includes \
        $test_file \
        ../src/math/vector.c \
        ../src/math/ray.c \
        ../src/math/intersections.c \
        ../src/parsing/parse_utils.c \
        ../src/parsing/parse_objects.c \
        ../src/parsing/parse_camera.c \
        ../src/parsing/parse_lights.c \
        ../src/utils/memory.c \
        ../src/utils/error.c \
        ../src/utils/colors.c \
        -lm -o test_runner 2>/dev/null
    
    if [ $? -eq 0 ]; then
        # Run the test
        ./test_runner
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✓ $test_name PASSED${NC}"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗ $test_name FAILED${NC}"
            FAILED=$((FAILED + 1))
        fi
        rm -f test_runner
    else
        echo -e "${RED}✗ $test_name COMPILATION FAILED${NC}"
        FAILED=$((FAILED + 1))
    fi
    echo ""
}

# Run all tests
echo "Starting unit tests..."
echo ""

run_test "Vector Operations" "test_vector.c"
run_test "Ray Operations" "test_ray.c"
run_test "Intersection Algorithms" "test_intersections.c"
run_test "Parsing Functions" "test_parsing_comprehensive.c"

# Integration test (requires more source files)
echo -e "${YELLOW}Running Integration Tests...${NC}"
gcc -Wall -Wextra -Werror -I.. -I../includes \
    test_integration.c \
    ../src/math/vector.c \
    ../src/math/ray.c \
    ../src/math/intersections.c \
    ../src/parsing/parse_utils.c \
    ../src/parsing/parse_objects.c \
    ../src/parsing/parse_camera.c \
    ../src/parsing/parse_lights.c \
    ../src/parsing/parse_scene.c \
    ../src/rendering/raytracer.c \
    ../src/rendering/lighting.c \
    ../src/rendering/reflections.c \
    ../src/utils/memory.c \
    ../src/utils/error.c \
    ../src/utils/colors.c \
    -lm -o test_integration 2>/dev/null

if [ $? -eq 0 ]; then
    ./test_integration
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Integration Tests PASSED${NC}"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}✗ Integration Tests FAILED${NC}"
        FAILED=$((FAILED + 1))
    fi
    rm -f test_integration
else
    echo -e "${RED}✗ Integration Tests COMPILATION FAILED${NC}"
    FAILED=$((FAILED + 1))
fi
echo ""

# Summary
echo "=== Test Summary ==="
echo -e "Tests passed: ${GREEN}$PASSED${NC}"
echo -e "Tests failed: ${RED}$FAILED${NC}"
echo -e "Total tests: $((PASSED + FAILED))"

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi