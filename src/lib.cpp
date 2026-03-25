#include "hashset.hpp"

HashSet::HashSet(size_t initial_size)
{
    bucket_count = initial_size;
    element_count = 0;
    load_threshold = 70;
    load_factor = 0;
    array = new LinkedList *[bucket_count];
    for (size_t i = 0; i < bucket_count; i++)
    {
        array[i] = new LinkedList();
    }
}

HashSet::~HashSet()
{
    clear();
    for (size_t i = 0; i < bucket_count; i++)
    {
        delete array[i];
    }
    delete[] array;
}

unsigned long HashSet::prehash(int item) const
{
    unsigned long h = 5381;
    h = ((h << 5) + h) + item;
    return h;
}

unsigned long HashSet::hash(unsigned long prehash) const
{
    return prehash % bucket_count;
}

// Insert item into the set. Returns true if inserted, false if already present.
// Rehashes if inserting will increase the load factor past the threshold.
bool HashSet::insert(int item)
{
    // Check if the item already exists in the set
    if (contains(item))
    {
        return false;
    }
    // Find the bucket index using hash function
    unsigned long bucket_index = hash(prehash(item));

    // Insert into the linked list at that bucket
    array[bucket_index]->insert(item);

    // Update element count and load factor
    element_count++;
    load_factor = (unsigned int)((double)element_count / bucket_count * 100);

    // Check if we need to rehash
    if (load_factor > load_threshold)
    {
        rehash(bucket_count * 2);
    }

    // Successfully inserted the item
    return true;
}

// Remove an item from the set. Returns true if removed, false if not found.
bool HashSet::remove(int item)
{
    // Find the bucket index using hash function
    unsigned long bucket_index = hash(prehash(item));

    // try to remove the item from the linked list at that bucket
    if (array[bucket_index] != nullptr)
    {
        // If the item was removed, update element count and load factor
        if (array[bucket_index]->remove(item))
        {
            element_count--;
            load_factor = (unsigned int)((1.0 * element_count / bucket_count) * 100);
            return true;
        }
    }
    // Item was not found in the set
    return false;
}

// Check if the item exists in the set
bool HashSet::contains(int item) const
{
    // Find the bucket index using hash function
    unsigned long bucket_index = hash(prehash(item));

    // Check if the item exists in the linked list at that bucket
    if (array[bucket_index] != nullptr)
    {
        return array[bucket_index]->contains(item);
    }
    // Item was not found in the set
    return false;
}

// Return the number of elements in the hash set
size_t HashSet::count() const
{
    return element_count;
}

// Return the current load factor as a percentage
unsigned int HashSet::load() const
{
    return load_factor;
}
