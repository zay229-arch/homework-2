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

/* Modify this prehash function to work with a int instead of a char*
unsigned long prehash(unsigned char *str) {
    unsigned long h = 5381;
    int c;
    while (c = *str++) { 
	    h = ((h << 5) + h) + c;
    }
    return h;
}*/
