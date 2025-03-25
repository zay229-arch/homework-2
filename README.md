# CSE 109 - Systems Software - Spring 2025

# Homework 3 - Implementing and Evaluating a Data Structure in C++

⏰ **Due Date: 4/4/2025 EOD**

## Instructions 

**Read thoroughly before starting your project:**

1. Fork this repository into your CSE109 project namespace. [Instructions](https://docs.gitlab.com/ee/workflow/forking_workflow.html#creating-a-fork)
2. Clone your newly forked repository onto your development machine. [Instructions](https://docs.gitlab.com/ee/gitlab-basics/start-using-git.html#clone-a-repository) 
3. As you are writing code you should commit patches along the way. *i.e.* don't just submit all your code in one big commit when you're all done. Commit your progress as you work. 

**💥IMPORTANT: You must commit frequently as you work on your project. As a general rule try to make at least one commit per function you implement.**

4. When you've committed all of your work, there's nothing left to do to submit the assignment.

## Hash Set Class

In assignment you will implement a hash set data structure in C++. The core of the hashset is an array of linked list pointers.

Hash sets are useful because they have very good performance in the average case. But they can be tempermental because ther are a lot of dials to tune to achieve this performance.

```
┌─────────────┬──────────┬───────────┐
│ Operation   │ Average  │ Worst Case│
├─────────────┼──────────┼───────────┤
│ Search      │ Θ(1)     │ O(n)      │
│ Insert      │ Θ(1)     │ O(n)      │
│ Delete      │ Θ(1)     │ O(n)      │
├─────────────┼──────────┼───────────┤
│ Space       │ Θ(n)     │ O(n)      │
└─────────────┴──────────┴───────────┘
```

The Hashset struct is declared in `hashset.h`, along with a number of functions you will need to implement.

```c++
class HashSet {
  private:
    // The backbone of the hash set: an array of linked list pointers for handling collisions
    LinkedList** array;

    // The number of buckets in the array
    size_t bucket_count; 

    // Total number of elements in the set
    size_t element_count;

    // Load factor threshold for resizing (default 70)
    unsigned int load_threshold;

    // The current load factor (average number of elements per bucket)
    unsigned int load_factor;

    // Resize the array by adjusting the number of buckets, rehashes all current elements
    void rehash(size_t new_size);

  public:
    // Initialize an empty hash set with a given number of buckets
    explicit HashSet(size_t initial_size);

    // Destructor: Free all allocated memory
    ~HashSet();

    // Generate a prehash for an item
    unsigned long prehash(int item) const;

    // Convert prehash value into a valid bucket index
    unsigned long hash(unsigned long prehash) const;

    // Insert item into the set. Returns true if inserted, false if already present.
    // Rehashes if inserting will increase the load factor past the threshold.
    bool insert(int item);

    // Remove an item from the set. Returns true if removed, false if not found.
    bool remove(int item);

    // Check if the item exists in the set
    bool contains(int item) const;

    // Return the number of elements in the hash set
    size_t count() const;

    // Return the current load factor as a percentage
    unsigned int load() const;

    // Set a new load factor threshold for resizing
    void set_load_threshold(unsigned int threshold);

    // Remove all elements from the hash set
    void clear();

    // Print the hash table (format is implementation-dependent)
    void print() const;
};

```

📝 Note: The load factor is stored as an unsigned integer so we can make comparisons easier. A load factor of 70 means that the average number of elements per bin is 0.7. When you recalculate the load factor, round to the nearest tenth and multiply by 100.

💡 Tip: Feel free to use your own linked list implementation or the HW2 solutions, but don't use a standard library linked list.

### Collision Resolution

There are many ways of dealing with collisions - when two items hash to the same bucket. For this assignment, we will use the linked-list chain method; each bucket contains a linked list, which starts empty, and grows every time an item is inserte and hashes to that bucket. When the item needs to be found or removed, the typical linked list find/remove is used.

"But doesn't that mean the hash set just performs like a linked list?" you might wonder. No, as long as we keep those linked list chains short. That's where the load factor comes in. As long as that stays low, then it won't take long to search through the whole list. 

Conceptually, a `LinkedList**` is an array in which each element is a pointer to a `LinkedList`. It's analogous to a `char**`, which we encountered with `argv`. In that case, it was an array of strings. The first star indicates it's a pointer to an array, the second star indicates each array element holds a LinkedList pointer.

Here's a diagram of a hash set of size 8 holding 11 elements might look:

```
---------------------------------
Bucket | Linked List
---------------------------------
  0    |  ⬜ → ⬜ → ⬜ → nullptr
  1    |  ⬜ → nullptr
  2    |  ⬜ → ⬜ → nullptr
  3    |  nullptr
  4    |  ⬜ → nullptr
  5    |  nullptr
  6    |  ⬜ → ⬜ → ⬜ → ⬜ → nullptr
  7    |  nullptr
---------------------------------
⬜ = Linked list node containing a stored value
```

## Makefile

Write a Makefile inside of the project root that has the following targets:

- all - Build both static and shared libraries.
- static - Build a static library `libhashset.a`, place it in `build/lib/release`. Store object files in `build/objects`.
- shared - Build a shared library `libhashset.so`, place it in `build/lib/release`. Store object files in `build/objects`.
- debug - Build a shared library with debug symbols (`libhashset.so`), place it in `build/lib/debug`. Store object files in `build/objects`.
- clean - Remove all build artifacts by deleting the `build` directory.
- install - Install the shared library to `/usr/local/lib`.
- test - Compile `tests/test.cpp`, place the executable in `build/bin`, store object files in `build/objects`, and run the tests.

## Validation

To validate the program behaves as expected, we consider both its functionality and performance.

### Functionality

Validate functionality with unit tests, which are small and verify operations like insertion, deletion, lookup, and resizing. Important aspects of writing good unit tests are:

- Test Coverage: do you test all the functionality of the hash set?
- Edge Cases: do you test edge cases such as duplicate insertions and removing non-existent items?

Write a C++ program that runs the tests located in the `/tests/tests` file. Your program should:

- Read in the test file from disk.
- Parse the test file to extract the byte code for each test.
- Iterate through each byte code string in the test file.
- For each byte code string,
  - Create a new hash set data structure
  - Iterate through the byte code and execute the indicarted hash set operation (insert, remove, contains, size, capacity).
  - Verify that the final state of the hash set is as expected.
  - Print the test result to the console. 
- Indicate whether the tests all pass, and if not, which ones failed.
- Exit with the number of tests that failed.

📝 Note: The tests file includes 10 example tests, but your program will be tested against more that are not included in the assignment.

### Performance

Performance tests measure execution time over large-scale inputs. As the number of elements contained in the set increase, we hope that the time to insert, find, or remove elements stays constant.

There is a file called `tests/bench.cpp`, which contains a framework for benchmarking datastructures using C++ timing features. It demonstrates this using the [`std::unordered_set`](https://en.cppreference.com/w/cpp/container/unordered_set), which you can use as a point of comparison against your hash set.

1. Set Up Experiment Parameters
    - Define different values of N.
    - Measure insertion, lookup, and deletion times separately.
2. Verify O(1) Growth
    - If times remain almost constant across increasing N, then operations are O(1).

```
┌──────────┬─────────────┬────────────────┬────────────┬──────┬───────────┐
│ Elements │ Load Factor │ Operation Time │ Collisions │ Size │ Rehashed? │
│     N    │      %      │      (ns)      │    Count   │ (MB) │   Yes/No  │
├──────────┼─────────────┼────────────────┼────────────┼──────┼───────────┤
│    500   │             │                │            │      │           │
│   1000   │             │                │            │      │           │
│   5000   │             │                │            │      │           │
│   7000   │             │                │            │      │           │
│  10000   │             │                │            │      │           │
└──────────┴─────────────┴────────────────┴────────────┴──────┴───────────┘
```

#### Collect Data

We need to collect data on the actual performance of the hash set, and we will compare our implementation against the standard library hash set. This is out data matrix:

- 2 data structures
    - Your `HashSet` implementation 
    - Control: `std::unordered_set`

- 3 Load factors
    - Load factor threshold 70
    - Load factor threshold 20    
    - Load factor threshold 120

- 3 Operations
    - Insert, 
    - Contains, 
    - Remove

2 Data structures x 3 Load factors x 3 operations x 5 max elements = 90 total experiments 

💡 Tip: Consider automating this with a test script.

#### Make Charts

From this, you will create the following charts:

1. Operation Performance vs. Element Count (Line Chart)
    - X-axis: Number of elements (log scale: 1K, 10K, 100K, 1M, 10M)
    - Y-axis: Average execution time (microseconds)
    - Lines:
        - 3 operations (insert, contains, remove)
        - 2 data structures (HashSet, std::unordered_set)
        - Total lines: 6
2. Load Factor Impact on Performance (Bar Chart)
    -  X-axis: Load factor thresholds (20, 70, 120)
    -  Y-axis: Execution time for each operation
    -  Bars:
        -  2 operations (insert, contains, remove)
        -  2 data structures (HashSet, std::unordered_set)
        -  Total bars: 6 per load factor × 3 load factors = 18

📝 Note: Proper charts have a title, labeled axes with units, grid lines, a legend, 

💡 Tip: when dealing with exponential data, a log scale can help visualize treds

## Evaluation

Deliverables:

- Source code for a hash set implemention written in C++, and a build script that compiles the code to static and dynamic libararies.
- An interpreter that evaluates the provided test file
- 20 additional tests
- A .gitlab-ci.yml script that runs your code against the provided tests.
- The two indicated charts, along with a writeup (approx 2-pages) on your procedures and findings. You may use LLMs for this, and it should be written in a Markdown file to replace this README file. Make sure your document displays your charts.

Some things to keep in mind:

- Only files under version control in your forked assignment repository will be graded. Local files left untracked on your computer will not be considered.

- Only code committed *and pushed* prior to the time of grading will be accepted. Locally committed but unpushed code will not be considered.

- Your assignment will be graded according to the [Programming Assignment Grading Rubric](https://drive.google.com/open?id=1V0nBt3Rz6uFMZ9mIaFioLF-48DFX0VdkbgRUDM_eIFk).

## Tips For Success 

Some things to take into consideration when writing your assignment:

- Start this project the day it's assigned. Use all the time allocated and don't create a situation where you are starting the project right before it is due.

- At the very least, make sure your code compiles (on the Sunlab machines) before you submit it.

- Use meaningful function and variable names. This will make the code easier to read and understand, and will also make it easier to maintain in the future.

- Adding comments to the code can help the graders understand what your code is doing, which can help them assign partial points to incorrect solutions.

- Always check for errors when reading input, opening files, or allocating memory. This will help prevent crashes and other unexpected behavior.

- Free resources like file handles, socket handles, heap memory, etc. as soon as they are not needed anymore to avoid resource leaks.

- Using the right data type can make your program more efficient and less error-prone. For example, use integers when working with whole numbers, and use floating-point numbers when working with decimal numbers. With systems software especially, we want to pay attention to this component of sofware design.

- Test your code: Before releasing your code, make sure to test it thoroughly. Try to anticipate how users will use your program and test it under different conditions to make sure it works as expected.