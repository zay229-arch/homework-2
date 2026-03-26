#include <iostream>
#include <fstream>
#include <string>
// a vector is a dynamic array that can grow in size. Will use it to store the test cases read from the file.
#include <vector>
#include "hashset.hpp"

// Declaration of the run_test function
// This function runs a single test case given as a string of bytecode and the test number for reporting purposes.
// Returns true if the test passed, false if it failed.
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

    // A vector to hold all of the test cases read from the file. Each test case is a string (a line from the file).
    std::vector<std::string> tests;
    // A string to hold each line as we read it from the file
    std::string line;

    // Read the file one line at a time
    // this increments the file stream with each call to getline, so we don't have to worry about it
    while (std::getline(file, line))
    {
        // Skip blank lines — nothing to test
        if (line.empty())
            continue;

        // Skip comment lines (lines that start with "--")
        if (line.size() >= 2 && line[0] == '-' && line[1] == '-')
            continue;

        // This line is a real test case — save it
        // push_back adds the line to the end of the tests vector
        tests.push_back(line);
    }

    // Counters to track how many tests pass or fail
    int passed = 0;
    int failed = 0;

    // Stores the test numbers of every test that failed, so we can report them at the end
    std::vector<int> failed_tests;

    // Run each test case in the tests vector. The index i is used to keep track of the test number (starting from 1).
    for (int i = 0; i < (int)tests.size(); i++)
    {
        // Call run_test with the test case and its number. If it returns true, the test passed; if false, it failed.
        // We send in the test number because it can be helpful for debugging to know which test case failed.
        if (run_test(tests[i], i + 1))
        {
            passed++; // run_test returned true — test passed
        }
        else
        {
            failed++;                      // run_test returned false — test failed
            failed_tests.push_back(i + 1); // remember which test number failed
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

    // Print a summary of the results
    // only print the summary after all tests have ran
    std::cout << "═════════════════════════════════" << std::endl;
    std::cout << "Results: " << passed << " passed, " << failed << " failed" << std::endl;
    std::cout << "═════════════════════════════════" << std::endl;

    // Return the number of failures as the exit code.
    // A return value of 0 means all tests passed.
    return failed;
}
