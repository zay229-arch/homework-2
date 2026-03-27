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

    // --- STL control: std::unordered_set ---
    std::cout << "=== STL std::unordered_set ===\n\n";
    for (int N : nValues)
    {
        std::unordered_set<int> hashSet;
        std::vector<int> testElements;

        size_t mem_before = get_memory_usage();

        auto start_insert = std::chrono::high_resolution_clock::now();
        for (int i = 1; i <= N; ++i)
            hashSet.insert(i);
        auto end_insert = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> insert_time = end_insert - start_insert;

        size_t mem_after = get_memory_usage();

        for (int i = 1; i <= N / 2; ++i)
            testElements.push_back(i);
        for (int i = N + 1; i <= N + N / 2; ++i)
            testElements.push_back(i);

        auto start_lookup = std::chrono::high_resolution_clock::now();
        for (int val : testElements)
            hashSet.count(val);
        auto end_lookup = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> lookup_time = end_lookup - start_lookup;

        auto start_remove = std::chrono::high_resolution_clock::now();
        for (int i = 1; i <= N; ++i)
            hashSet.erase(i);
        auto end_remove = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> remove_time = end_remove - start_remove;

        std::cout << "N: " << N << "\n";
        std::cout << "  Insertion Time: " << insert_time.count() / N << " ns per element\n";
        std::cout << "  Lookup Time:    " << lookup_time.count() / testElements.size() << " ns per element\n";
        std::cout << "  Remove Time:    " << remove_time.count() / N << " ns per element\n";
        std::cout << "  Memory Used:    " << (mem_after - mem_before) / 1024.0 << " MB\n";
        std::cout << "\n";
    }

    // --- Custom HashSet ---
    std::cout << "=== Custom HashSet ===\n\n";
    for (unsigned int threshold : loadThresholds)
    {
        for (int N : nValues)
        {
            HashSet hashSet(16);
            hashSet.set_load_threshold(threshold);

            std::vector<int> testElements;

            size_t mem_before = get_memory_usage();

            auto start_insert = std::chrono::high_resolution_clock::now();
            for (int i = 1; i <= N; ++i)
                hashSet.insert(i);
            auto end_insert = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::nano> insert_time = end_insert - start_insert;

            size_t mem_after = get_memory_usage();

            for (int i = 1; i <= N / 2; ++i)
                testElements.push_back(i);
            for (int i = N + 1; i <= N + N / 2; ++i)
                testElements.push_back(i);

            auto start_lookup = std::chrono::high_resolution_clock::now();
            for (int val : testElements)
                hashSet.contains(val);
            auto end_lookup = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::nano> lookup_time = end_lookup - start_lookup;

            auto start_remove = std::chrono::high_resolution_clock::now();
            for (int i = 1; i <= N; ++i)
                hashSet.remove(i);
            auto end_remove = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::nano> remove_time = end_remove - start_remove;

            std::cout << "Load Threshold: " << threshold << "% | N: " << N << "\n";
            std::cout << "  Insertion Time: " << insert_time.count() / N << " ns per element\n";
            std::cout << "  Lookup Time:    " << lookup_time.count() / testElements.size() << " ns per element\n";
            std::cout << "  Remove Time:    " << remove_time.count() / N << " ns per element\n";
            std::cout << "  Memory Used:    " << (mem_after - mem_before) / 1024.0 << " MB\n";
            std::cout << "  Collisions:     " << hashSet.collisions() << "\n";
            std::cout << "  Rehashes:       " << hashSet.rehashes() << "\n";
            std::cout << "\n";
        }
    }

    // --- Chart Data: Line Chart (Operation Time vs N, threshold=70) ---
    // This section uses larger N values to match the line chart x-axis: 1K, 10K, 100K, 1M, 10M.
    // These are log-scale steps that reveal how performance scales as the set grows.
    // We fix the load threshold at 70% (the default) so the only variable is N.
    // Output is in microseconds (ns / 1000) since that's the unit the chart requires.
    std::vector<int> chartN = {1000, 10000, 100000, 1000000, 10000000};

    std::cout << "=== CHART DATA: Line Chart (threshold=70) ===\n\n";

    // --- STL line chart data ---
    // Run std::unordered_set at each N so we have a control line on the chart.
    // A fresh set is created each iteration to avoid carryover state between N values.
    std::cout << "[STL]\n";
    for (int N : chartN)
    {
        std::unordered_set<int> hashSet;
        std::vector<int> testElements;

        // Insert 1..N into the STL set and time the total insertion phase
        auto start_insert = std::chrono::high_resolution_clock::now();
        for (int i = 1; i <= N; ++i)
            hashSet.insert(i);
        auto end_insert = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> insert_time = end_insert - start_insert;

        // Build a mixed lookup list: first half are present (1..N/2),
        // second half are absent (N+1..N+N/2). This gives a realistic 50/50 hit rate.
        for (int i = 1; i <= N / 2; ++i)
            testElements.push_back(i);
        for (int i = N + 1; i <= N + N / 2; ++i)
            testElements.push_back(i);

        // Time how long it takes to look up all test elements.
        // Note: the compiler may optimize this away since the result of count() is unused.
        auto start_lookup = std::chrono::high_resolution_clock::now();
        for (int val : testElements)
            hashSet.count(val);
        auto end_lookup = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> lookup_time = end_lookup - start_lookup;

        // Time removal of all inserted elements
        auto start_remove = std::chrono::high_resolution_clock::now();
        for (int i = 1; i <= N; ++i)
            hashSet.erase(i);
        auto end_remove = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> remove_time = end_remove - start_remove;

        // Divide total time by N (or testElements.size()) to get per-element cost.
        // Divide by 1000 to convert nanoseconds -> microseconds for the chart.
        std::cout << "N=" << N
                  << " insert=" << insert_time.count() / N / 1000.0 << "us"
                  << " lookup=" << lookup_time.count() / testElements.size() / 1000.0 << "us"
                  << " remove=" << remove_time.count() / N / 1000.0 << "us\n";
    }

    // --- Custom HashSet line chart data ---
    // Same experiment repeated with our HashSet at threshold=70 so we can compare
    // the two lines directly on the chart.
    std::cout << "\n[HashSet threshold=70]\n";
    for (int N : chartN)
    {
        // Start with 16 buckets; the set will rehash automatically as N grows
        HashSet hashSet(16);
        hashSet.set_load_threshold(70);

        std::vector<int> testElements;

        // Insert 1..N and measure total insertion time (includes any rehash cost)
        auto start_insert = std::chrono::high_resolution_clock::now();
        for (int i = 1; i <= N; ++i)
            hashSet.insert(i);
        auto end_insert = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> insert_time = end_insert - start_insert;

        // Same 50/50 present/absent lookup list as the STL run above
        for (int i = 1; i <= N / 2; ++i)
            testElements.push_back(i);
        for (int i = N + 1; i <= N + N / 2; ++i)
            testElements.push_back(i);

        // Time contains() across all test elements
        auto start_lookup = std::chrono::high_resolution_clock::now();
        for (int val : testElements)
            hashSet.contains(val);
        auto end_lookup = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> lookup_time = end_lookup - start_lookup;

        // Time removal of all N inserted elements
        auto start_remove = std::chrono::high_resolution_clock::now();
        for (int i = 1; i <= N; ++i)
            hashSet.remove(i);
        auto end_remove = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::nano> remove_time = end_remove - start_remove;

        // Per-element cost in microseconds
        std::cout << "N=" << N
                  << " insert=" << insert_time.count() / N / 1000.0 << "us"
                  << " lookup=" << lookup_time.count() / testElements.size() / 1000.0 << "us"
                  << " remove=" << remove_time.count() / N / 1000.0 << "us\n";
    }

    return 0;
}
