#include "linkedlist.hpp"

// Constructs an empty list
LinkedList ::LinkedList()
{
    head = nullptr;
}

// Destructor: delete all nodes when the list is destroyed
LinkedList ::~LinkedList()
{
    clear();
}

// Insert a value into the list
// Returns false if the value already exists
bool LinkedList::insert(int item)
{
    // Do not allow duplicates
    if (contains(item))
    {
        return false;
    }

    // Create a new node
    Node *newNode = new Node;

    newNode->value = item;
    // Point the new node to the old head
    newNode->next = head;

    // Insert at the front of the list
    head = newNode;

    return true;
}
