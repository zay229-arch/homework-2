#include <iostream>
#include <unordered_set> // This is the hash set from the C++ STL. Use it as a point of comparison.
#include <chrono>
#include <vector>
#include <sys/resource.h> // For getrusage() on Linux/macOS
#include "hashset.hpp"

// Function to get memory usage (Linux/macOS)
size_t get_memory_usage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // Memory in kilobytes
}

int main()
{
    std::vector<int> nValues = {500, 1000, 5000, 7000, 10000};
    std::vector<unsigned int> loadThresholds = {20, 70, 120};

    for (unsigned int threshold : loadThresholds)
    {
        for (int N : nValues)
        {
            // std::unordered_set<int> hashSet;
            HashSet hashSet(16);
            hashSet.set_load_threshold(threshold);

            std::vector<int> testElements;

            // Measure memory before insertion
            size_t mem_before = get_memory_usage();

            // Measure insertion time
            auto start_insert = std::chrono::high_resolution_clock::now();
            for (int i = 1; i <= N; ++i)
            {
                hashSet.insert(i);
            }
            auto end_insert = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> insert_time = end_insert - start_insert;

            // Measure memory after insertion
            size_t mem_after = get_memory_usage();

            // Prepare test elements (half present, half absent)
            for (int i = 1; i <= N / 2; ++i)
                testElements.push_back(i); // Present elements
            for (int i = N + 1; i <= N + N / 2; ++i)
                testElements.push_back(i); // Absent elements

            // Measure lookup time
            auto start_lookup = std::chrono::high_resolution_clock::now();
            for (int val : testElements)
            {
                hashSet.contains(val);
            }
            auto end_lookup = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> lookup_time = end_lookup - start_lookup;

            // Output results
            std::cout << "Load Threshold: " << threshold << "% | N: " << N << "\n";
            std::cout << "  Insertion Time: " << insert_time.count() / N << " sec per element\n";
            std::cout << "  Lookup Time:    " << lookup_time.count() / testElements.size() << " sec per element\n";
            std::cout << "  Memory Used:    " << (mem_after - mem_before) / 1024.0 << " MB\n";
            std::cout << "\n";
        }
    }

    return 0;
}
