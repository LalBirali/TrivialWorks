#pragma once 
#include "object.h"
#include <iostream>
#include <cmath>

/**
 * This class is meant to be a wrapper class that contains an float.
 * This class extends the Object
 * Author: kenright12345 <enright.ke@husky.neu.edu>, lalbirali
 * <birali.b@husky.neu.edu>
 */
class Float : public Object {
public:
    float stored;
    /**
     * Basic Constructor for a Float, defaults to zero
     */
    Float() {
        stored = 0;
    }

     /**
     * Constructor for an Float, wraps the argument taken in within this class
     * @arg f float to be wrapped within this class
     */
    Float(float f) {
        stored = f;
    }

    /**
     * Destructor for float
     */
    ~Float(){}

    /**
     *  Check if this Float "equals" Object other
     *  @param other object to compare to
     *  @return true if they are equal else false
     */
    bool equals(Object* other) {
        if (other == this) return true;
        Float* x = dynamic_cast<Float *>(other);
        if (x == nullptr) return false;
        return (stored == x->stored);
    }

    /**
     * Return the hash of this Float.
     * @return the hashcode
     */
    size_t hash() {
     if (stored < 0) {
            return floor(stored) * -1;
        } else {
            return floor(stored);
        }   
    }

    /**
     * Prints out the wrapped float to console.
     */
    void print() {
        std::cout << stored << std::endl;
    }

    /**
     * Get method for the wrapped float
     * @return the wrapped float
     */
    float get() {
        return stored;
    }

};