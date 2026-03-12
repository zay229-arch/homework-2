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

// Remove a value from the list
// Returns true if removed, false if not found
bool LinkedList::remove(int item)
{
    Node *current = head;
    Node *previous = nullptr;

    // Go through the list
    while (current != nullptr)
    {

        if (current->value == item)
        {

            // If removing the first node
            if (previous == nullptr)
            {
                head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current;
            return true;
        }

        previous = current;
        current = current->next;
    }
    return false;
}
