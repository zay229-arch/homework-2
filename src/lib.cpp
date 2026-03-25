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

