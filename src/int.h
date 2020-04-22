#pragma once 
#include "object.h"

/**
 * This class is meant to be a wrapper class that contains an int.
 * This class extends the Object
 * Author: kenright12345 <enright.ke@husky.neu.edu>, lalbirali
 * <birali.b@husky.neu.edu>
 */
class Int : public Object {
public:
    int stored;
    /**
     * Basic Constructor for an Int, defaults to zero
     */
    Int() {
        stored = 0;
    }

     /**
     * Constructor for an Int, wraps the argument taken in within this class
     * @arg i integer to be wrapped within this class
     */
    Int(int i) {
        stored = i;
    }

    /**
     * Destructor for Int
     */
    ~Int(){}

    /**
     *  Check if this Int "equals" Object other
     *  @param other object to compare to
     *  @return true if they are equal else false
     */
    bool equals(Object* other) {
        if (other == this) return true;
        Int* x = dynamic_cast<Int *>(other);
        if (x == nullptr) return false;
        return (stored == x->stored);
    }

    /**
     * Return the hash of this Int.
     * @return the hashcode
     */
    size_t hash() {
        if (stored < 0) {
            return stored * -1;
        } else {
            return stored;
        }
    }

    /**
     * Prints out the wrapped integer to console.
     */
    void print() {
        std::cout << stored << std::endl;
    }

    /**
     * Get method for the wrapped integer
     * @return the wrapped integer
     */
    int get() {
        return stored;
    }
};