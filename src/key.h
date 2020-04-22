#pragma once

#include <string>
#include "object.h"
#include <stdlib.h>

/**
Key::
This class represents a Key, which is represented by a string (name) and a node number.
During the networking part of this assignment, the int node will represent the sd of that node.
*/
class Key : public Object{
    public:
    std::string name;
    int node;
    /**
    Default constructor for a key
    */
    Key() {
        name = "";
        node = -1;
    }

    /**
    Constructor for a key that takes in a string name and an int node.
    */
    Key(std::string name, int node) {
        this->name = name;
        this->node = node;
    }

    /**
    Destructor for a Key
    */
    ~Key() {}

    /**
    Compares this Key to Object other and determines if they are equal.
    */
    bool equals(Object* other) {
        if (other == this) return true;
        Key* x = dynamic_cast<Key*>(other);
        if (x == nullptr) return false;
        if (this->name == x->name && this->node == x->node) {
            return true;
        } else {
            return false;
        }
    }
};