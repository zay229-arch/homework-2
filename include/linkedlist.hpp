#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <cstddef>
#include <iostream>

struct Node
{
    int value;
    Node *next;
};

class LinkedList
{
private:
    Node *head;

public:
    LinkedList();
    ~LinkedList();

    bool insert(int item);
    bool remove(int item);
    bool contains(int item) const;
    void clear();
    void print() const;
};

#endif