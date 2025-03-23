#include <iostream>
#include <unordered_set>  // This is the hash set from the C++ STL. Use it as a point of comparison. 
#include <chrono>
#include <vector>
#include <sys/resource.h> // For getrusage() on Linux/macOS
// #include <YOUR_HASH_SET_HEADER> // Include your custom hash set header here

// Function to get memory usage (Linux/macOS)
size_t get_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // Memory in kilobytes
}

int main() {
    const int N = 10000000; // Number of elements, we can change this parameter
    std::unordered_set<int> hashSet; // This is the std hash set, it's a template class so the <int> indicates the hash set contains integers
    // HashSet hashSet; // Replace with your custom hash set type
    std::vector<int> testElements;
    
    // Measure memory before insertion
    size_t mem_before = get_memory_usage();

    // Measure insertion time
    auto start_insert = std::chrono::high_resolution_clock::now();
    for (int i = 1; i <= N; ++i) {
        hashSet.insert(i);
    }
    auto end_insert = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> insert_time = end_insert - start_insert;
    
    // Measure memory after insertion
    size_t mem_after = get_memory_usage();

    // Prepare test elements (half present, half absent)
    for (int i = 1; i <= N / 2; ++i) testElements.push_back(i);         // Present elements
    for (int i = N + 1; i <= N + N / 2; ++i) testElements.push_back(i); // Absent elements

    // Measure lookup time
    auto start_lookup = std::chrono::high_resolution_clock::now();
    for (int val : testElements) {
        // Find out if the hash set contains val.
    }
    auto end_lookup = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> lookup_time = end_lookup - start_lookup;

    // Output results
    std::cout << "Insertion Time: " << insert_time.count() / N << " sec per element\n";
    std::cout << "Lookup Time: " << lookup_time.count() / testElements.size() << " sec per element\n";
    std::cout << "Memory Used: " << (mem_after - mem_before) / 1024.0 << " MB\n";

    return 0;
}
