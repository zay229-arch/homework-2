#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

// cstddef provides size_t and other standard types
#include <cstddef>

// Node structure representing one element in the linked list.
struct Node
{
    int value;
    Node *next; // pointer to the next node
};

// LinkedList class used for chaining in the hash set.
class LinkedList
{
private:
    Node *head;

public:
    LinkedList();  // creates empty list
    ~LinkedList(); // frees all dynamically allocated nodes

    bool insert(int item);         // add item to list
    bool remove(int item);         // remove item from list
    bool contains(int item) const; // check if item exists in list
    void clear();                  // delete all nodes
    void print() const;            // print list contents
    Node *get_head() const;        // getter for head pointer (used in rehashing)
};

#endif
