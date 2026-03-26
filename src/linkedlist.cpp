#include "linkedlist.hpp"

// iostream allows printing with std::cout
#include <iostream>

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

// Check if a value exists in the list
bool LinkedList::contains(int item) const
{
    Node *current = head;

    while (current != nullptr)
    {
        if (current->value == item)
            return true;
        current = current->next;
    }
    return false;
}

// Delete every node in the list
void LinkedList::clear()
{

    Node *current = head;

    while (current != nullptr)
    {
        Node *temp = current;
        current = current->next;

        delete temp;
    }
    head = nullptr;
}

// Print the list (can use for debugging)
void LinkedList::print() const
{
    Node *current = head;

    while (current != nullptr)
    {
        //(cout means console output.)
        std::cout << current->value << " -> ";
        current = current->next;
    }
    // std::endl means end line (move to next line)
    std::cout << "nullptr" << std::endl;
}

//getter for head pointer (used in rehashing)
Node *LinkedList::get_head() const
{
    return head;
}  
