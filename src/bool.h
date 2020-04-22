#pragma once 
#include "object.h"
#include <iostream>

/**
 * This class is meant to be a wrapper class that contains an bool.
 * This class extends the Object
 * Author: kenright12345 <enright.ke@husky.neu.edu>, lalbirali
 * <birali.b@husky.neu.edu>
 */
class Bool : public Object {
public:
    bool stored;
    /**
     * Basic Constructor for a bool, defaults to zero
     */
    Bool() {
        stored = false;
    }

     /**
     * Constructor for an Bool, wraps the argument taken in within this class
     * @arg b bool to be wrapped within this class
     */
    Bool(bool b) {
        stored = b;
    }

    /**
     * Destructor for float
     */
    ~Bool(){}

    /**
     *  Check if this Bool "equals" Object other
     *  @param other object to compare to
     *  @return true if they are equal else false
     */
    bool equals(Object* other) {
        if (other == this) return true;
        Bool* x = dynamic_cast<Bool *>(other);
        if (x == nullptr) return false;
        return ((stored && x->stored) || (!stored && !x->stored));
    }

    /**
     * Return the hash of this Bool.
     * @return the hashcode
     */
    size_t hash() {
        if (stored) {
            return 7;
        } else {
            return 17;
        }
    }

    /**
     * Prints out the wrapped bool to console.
     */
    void print() {
        std::cout << stored << std::endl;
    }

    /**
     * Get method for the wrapped bool
     * @return the wrapped bool
     */
    bool get() {
        return stored;
    }
};