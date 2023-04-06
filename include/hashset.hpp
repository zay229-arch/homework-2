class HashSet {
  private:
    // The backbone of the hash set. This is an array of Linked List pointers.
    LinkedList** array;

    // The number of buckets in the array
    size_t size; 

    // Generate a prehash for an item with a given size
    unsigned long prehash(int item);

  public:
    // Initialize an empty hash set, where size is the number of buckets in the array
    HashSet(size_t size);

    // Free all memory allocated by the hash set
    ~HashSet();

    // Hash an unsigned long into an index that fits into a hash set
    unsigned long hash(int item);

    // Insert item in the set. Return true if the item was inserted, false if it wasn't (i.e. it was already in the set)
    // Recalculate the load factor after each successful insert (round to nearest whole number).
    // If the load factor exceeds 70 after insert, resize the table to hold twice the number of buckets.
    bool insert(int item);

    // Remove an item from the set. Return true if it was removed, false if it wasn't (i.e. it wasn't in the set to begin with)
    bool remove(int item);

    // Return true if the item exists in the set, false otherwise
    bool contains(int item);

    // Returns the number of items in the hash set
    size_t len();

    // Returns the number of empty buckets that can be filled before reallocating
    size_t capacity();

    // Print Table. You can do this in a way that helps you implement your hash set.
    void print();

};

/* Modify this prehash function to work with a template type T instead of a char*
unsigned long prehash(unsigned char *str) {
    unsigned long h = 5381;
    int c;
    while (c = *str++) { 
	    h = ((h << 5) + h) + c;
    }
    return h;
}*/

