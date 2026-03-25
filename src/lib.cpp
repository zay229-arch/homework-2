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