#include <iostream>
#include <fstream>
#include <string>

// vector is a dynamic array that can resize itself automatically when elements are added or removed.
#include <vector>
#include "hashset.hpp"

bool run_test(const std::string &bytecode, int test_num);

int main()
{
    // Open the test file that contains all of the test cases
    std::ifstream file("tests/tests");

    // If the file couldn't be opened (wrong path, missing file, etc.), bail out early
    if (!file.is_open())
    {
        std::cerr << "Error: could not open tests/tests" << std::endl;
        return 1;
    }

    // Counters to track how many tests pass or fail
    int passed = 0;
    int failed = 0;
    int test_num = 0;

    // Stores the test numbers of every test that failed, so we can report them at the end
    std::vector<int> failed_tests;

    // A string to hold each line as we read it from the file
    std::string line;

    // Read, parse, and run each test in a single pass.
    // getline advances the file stream by one line each call — no manual incrementing needed.
    while (std::getline(file, line))
    {
        // Skip blank lines
        if (line.empty())
            continue;

        // Skip comment lines (lines that start with "--")
        if (line.size() >= 2 && line[0] == '-' && line[1] == '-')
            continue;

        // This line is a real test case
        test_num++;

        // Call run_test with the bytecode and its test number.
        // We send in the test number because it can be helpful for debugging to know which test case failed.
        if (run_test(line, test_num))
        {
            passed++; // run_test returned true — test passed
        }
        else
        {
            failed++;                         // run_test returned false — test failed
            failed_tests.push_back(test_num); // remember which test number failed
        }
    }

    // If any tests failed, list their numbers so the user knows exactly which ones to look at
    if (!failed_tests.empty())
    {
        std::cout << "Failed tests: ";
        for (int i = 0; i < (int)failed_tests.size(); i++)
        {
            if (i > 0)
                std::cout << ", ";
            std::cout << "#" << failed_tests[i];
        }
        std::cout << std::endl;
    }

    // Print a summary of the results — only printed after all tests have run
    std::cout << "═════════════════════════════════" << std::endl;
    std::cout << "Results: " << passed << " passed, " << failed << " failed" << std::endl;
    std::cout << "═════════════════════════════════" << std::endl;

    // Return the number of failures as the exit code.
    // A return value of 0 means all tests passed.
    return failed;
}
