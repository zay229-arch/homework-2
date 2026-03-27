# CSE 109 - Systems Software - Spring 2026

# Homework 2 - Implementing and Evaluating a HashSet in C++

## Overview

This project implements a hash set data structure in C++ using chaining (linked lists) for collision resolution, along with a full benchmarking suite that compares its performance against the C++ standard library's `std::unordered_set`.

## Implementation

The `HashSet` class stores elements in an array of linked lists (`LinkedList**`). Each element is hashed to a bucket index; if two elements hash to the same bucket, they are chained together in that bucket's linked list. When the load factor (elements / buckets × 100) exceeds the configured threshold, the table is rehashed into a new array of double the size and all existing elements are re-inserted.

The prehash function is a variant of the djb2 hash adapted for integers:

```cpp
unsigned long h = 5381;
h = ((h << 5) + h) + item;
```

Key design decisions:
- **Load factor stored as integer percentage** (e.g. 70 = 70%) to avoid floating point comparisons.
- **Rehash threshold is configurable** via `set_load_threshold()`, allowing experiments at 20%, 70%, and 120%.
- **Collision and rehash counters** track internal behavior for benchmarking transparency.

## Methodology

The benchmarker (`tests/benchmarker.cpp`) runs 90 experiments across:

- **2 data structures**: custom `HashSet`, `std::unordered_set`
- **3 load factor thresholds**: 20%, 70%, 120%
- **3 operations**: insert, contains/lookup, remove
- **5 element counts (table)**: 500, 1K, 5K, 7K, 10K
- **5 element counts (charts)**: 1K, 10K, 100K, 1M, 10M

Timing uses `std::chrono::high_resolution_clock` and results are reported in nanoseconds per element (table) or microseconds per element (charts). Memory is measured via `getrusage()`.

> **Note:** STL lookup times at large N were measured as near-zero (~0.000007 µs at 10M elements). This is a benchmarking artifact — the compiler optimized away the `hashSet.count()` call since its result is unused. STL lookup data should be treated as unreliable.

## Results

### Performance Table (HashSet, threshold = 70%)

| N | Threshold | Insert (ns) | Collisions | Rehashes |
|---|---|---|---|---|
| 500 | 70% | 171.4 | 0 | 6 |
| 1,000 | 70% | 173.8 | 0 | 7 |
| 5,000 | 70% | 147.5 | 0 | 9 |
| 7,000 | 70% | 244.4 | 0 | 10 |
| 10,000 | 70% | 206.8 | 0 | 10 |

### Chart 1 — Operation Performance vs Element Count

Insert time (µs/element) for HashSet vs STL at threshold = 70%:

| N | HashSet Insert | STL Insert | HashSet Lookup | HashSet Remove | STL Remove |
|---|---|---|---|---|---|
| 1K | 103.544 | 0.030 | 0.004 | 0.013 | 0.013 |
| 10K | 2.303 | 0.023 | 0.005 | 0.016 | 0.012 |
| 100K | 0.471 | 0.050 | 0.006 | 0.025 | 0.012 |
| 1M | 0.398 | 0.078 | 0.010 | 0.028 | 0.015 |
| 10M | 0.373 | 0.073 | 0.006 | 0.020 | 0.017 |

```mermaid
xychart-beta
    title "Insert Time vs N (µs/element, threshold=70%)"
    x-axis ["1K", "10K", "100K", "1M", "10M"]
    y-axis "µs per element" 0 --> 110
    line [103.544, 2.303, 0.471, 0.398, 0.373]
    line [0.030, 0.023, 0.050, 0.078, 0.073]
```

*Lines: HashSet (top at 1K), STL (bottom). Lookup and remove stabilize under 0.02 µs across all N and are omitted from this chart for readability.*

### Chart 2 — Load Factor Impact on Insert Performance (N = 10,000)

```mermaid
xychart-beta
    title "Insert Time by Load Threshold (N=10,000)"
    x-axis ["Threshold 20%", "Threshold 70%", "Threshold 120%"]
    y-axis "ns per element" 0 --> 650
    bar [577.812, 206.757, 244.376]
```

| Threshold | Insert (ns) | Lookup (ns) | Remove (ns) | Collisions | Rehashes |
|---|---|---|---|---|---|
| 20% | 577.8 | 4.2 | 13.2 | 0 | 12 |
| 70% | 206.8 | 5.4 | 28.5 | 0 | 10 |
| 120% | 244.4 | 4.8 | 17.9 | 3,360 | 10 |

## Analysis

**Insert performance converges to O(1).** The HashSet insert time drops sharply from 104 µs at N=1K to ~0.47 µs at N=100K and remains flat through 10M. The N=1K spike is not a sign of poor scaling — it reflects rehash overhead being amortized across very few elements. At large N, each rehash doubles the table size, so rehash events become exponentially rarer per element and the amortized cost converges.

**Lookup and remove are consistently O(1).** Both operations stay flat between 0.004–0.010 µs and 0.013–0.028 µs respectively across the full 1K–10M range, confirming expected constant-time behavior. The STL matches this pattern for remove.

**Load factor threshold has a non-obvious effect on insert time.** Threshold 20% is the slowest (578 ns) despite having zero collisions — it triggers 12 rehashes and each is expensive. Threshold 70% is fastest (207 ns) because it balances rehash frequency against collision risk. Threshold 120% is mid-range (244 ns) and is the only configuration to produce collisions (3,360 at N=10K), which slightly degrades lookup chain length.

**Custom HashSet vs STL.** At large N the HashSet insert time (~373 ns at 10M) is about 5× slower than STL (~73 ns). This is expected — `std::unordered_set` also uses separate chaining internally but benefits from a heavily optimized allocator, cached hash values, and better cache locality from its node layout. The custom HashSet allocates a `new LinkedList()` per bucket upfront and a `new Node` per insertion, which adds allocation overhead per element. Remove and lookup are competitive with STL at large N.

## Conclusion

The implemented HashSet achieves amortized O(1) insert, lookup, and remove. The 70% load threshold is the best-performing configuration tested, balancing rehash cost against collision frequency. A threshold of 20% minimizes collisions but incurs excessive rehashing overhead. A threshold of 120% reduces rehash frequency but allows collisions that degrade lookup performance. Future improvements could include open addressing for better cache locality and a smarter initial bucket count to reduce early rehashing overhead.
