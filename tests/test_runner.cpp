#include <iostream>
#include <string>
#include "../include/hashset.hpp"

// Returns true if every assertion in the test passed, or false if any assertion failed.
bool run_test(const std::string &bytecode, int test_num)
{
    int i = 0; // current position in the bytecode string
    int n = bytecode.size();

    // The HashSet we are testing. Created by the 'H' opcode and deleted by the 'X' opcode.
    // an opcode is a single character that says what operation to perform (e.g. 'H' for create HashSet, 'I' for insert, etc.)
    HashSet *hs = nullptr;

    // Stays true until an assertion fails.
    bool passed = true;

    // Each iteration of this loop processes one instruction.
    // An instruction is an opcode letter (e.g. 'H', 'I') optionally followed by an integer argument.
    // the integer is a number that the opcode uses for its operation (e.g. the number of buckets for 'H', the value to insert for 'I', etc.)
    while (i < n)
    {
        // Read the opcode — a single character that says what operation to perform.
        char opcode = bytecode[i];
        i++;

        // --- Parse the integer argument that follows the opcode (if any) ---

        // arg will hold the integer argument for the opcode, if it has one. If the opcode doesn't take an argument, this variable will be ignored.
        int arg = 0;
        bool negative = false;

        // A '-' character means the argument is a negative number.
        if (i < n && bytecode[i] == '-')
        {
            negative = true;
            i++;
        }

        // Build the integer one digit at a time.
        // e.g. for "123", first pass: arg = 1, second: arg = 12, third: arg = 123
        // The condition bytecode[i] >= '0' && bytecode[i] <= '9' checks if the current character is a digit.
        while (i < n && bytecode[i] >= '0' && bytecode[i] <= '9')
        {
            // Shift the current digits in arg to the left (multiply by 10) and add the new digit.
            arg = arg * 10 + (bytecode[i] - '0');
            i++;
        }
        // If there was a '-' sign, make the argument negative.
        if (negative)
            arg = -arg;

        // --- Dispatch: decide what to do based on the opcode ---

        switch (opcode)
        {
        // H <size> — create a new HashSet with the given number of buckets
        case 'H':
            hs = new HashSet(arg);
            break;

        // I <value> — insert value into the set
        case 'I':
            hs->insert(arg);
            break;

        // R <value> — remove value from the set
        case 'R':
            hs->remove(arg);
            break;

        // C <value> — assert the value IS in the set; fail if it isn't
        case 'C':
            if (!hs->contains(arg))
            {
                std::cout << "Test #" << test_num << " FAILED: expected " << arg << " to be in the set\n";
                passed = false;
            }
            break;

        // D <value> — assert the value is NOT in the set; fail if it is
        case 'D':
            if (hs->contains(arg))
            {
                std::cout << "Test #" << test_num << " FAILED: expected " << arg << " to NOT be in the set\n";
                passed = false;
            }
            break;

        // S <size> — assert the set contains exactly <size> elements
        case 'S':
            // hs->count() returns the number of elements currently in the set. We compare it to the expected size (arg).
            if ((int)hs->count() != arg)
            {
                std::cout << "Test #" << test_num << " FAILED: expected size " << arg
                          << " but got " << hs->count() << "\n";
                passed = false;
            }
            break;

        // L <load> — assert the current load factor equals <load>
        case 'L':
            if ((int)hs->load() != arg)
            {
                std::cout << "Test #" << test_num << " FAILED: expected load " << arg
                          << " but got " << hs->load() << "\n";
                passed = false;
            }
            break;

        // X — end of test; free the HashSet and return the result
        case 'X':
            delete hs;
            return passed;
        }
    }

    // If we reach here the bytecode had no 'X' terminator — clean up and return.
    delete hs;
    return passed;
}
