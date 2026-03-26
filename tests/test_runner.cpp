#include <string>
#include "hashset.hpp"

// Returns true if the test passed, false if it failed.
bool run_test(const std::string &bytecode, int test_num)
{
    int i = 0;
    int n = bytecode.size();

    // A loop to parse the bytecode string. Each instruction consists of an opcode (a single character) followed by an optional integer argument.
    while (i < n)
    {
        // Parse the opcode (the first character of the instruction)
        char opcode = bytecode[i];
        i++;

        // Parse the integer argument following the opcode (if any)
        // the integer argument represents a value that the instruction operates on, such as a number to push onto the stack or an address to jump to.
        int arg = 0;
        bool negative = false;

        // Check if the next character is a '-' sign, which indicates that the integer argument is negative. If so, set the negative flag and move past the '-' character.
        if (i < n && bytecode[i] == '-')
        {
            negative = true;
            i++;
        }

        // Parse the integer argument by reading characters until we encounter a non-digit character. 
        // For each digit character, we update the arg variable by multiplying the current value by 10 and adding the numeric value of the digit (which we get by subtracting '0' from the character).
        while (i < n && bytecode[i] >= '0' && bytecode[i] <= '9')
        {
            arg = arg * 10 + (bytecode[i] - '0');
            i++;
        }

        if (negative)
            arg = -arg;

        // TODO: dispatch on opcode
    }

    return false;
}
