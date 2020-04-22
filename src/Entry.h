#pragma once 
#include <string>
#include <string.h>
#include <stdlib.h>

using namespace std;

/**
 * An enum to represent the 4 types of input in the SoR file:
 * INT
 * BOOL
 * FLOAT
 * STRING
 **/
enum TYPE  {
    INT, BOOL, FLOAT, STRING, EMPTY
};


/**
 * This method is a utility method for the enum type declared. It is designed to help
 * With the user query to determine the column type for the SoR file. 
 **/
string* enumToString(TYPE t) {
    switch(t) {
        case INT: {
            return new string("INT");
        }
        case BOOL: {
            return new string("BOOL");
        }
        case FLOAT: {
            return new string("FLOAT");
        }
        case STRING: {
            return new string("STRING");
        }
        case EMPTY: {
            return new string("EMPTY");
        }
        default:
            return new string();
    }
}


class Entry {
    public:
    string* str = new string("");
    bool bl = false;
    int i = -1;
    float f = -1.0;
    TYPE type;

    ~Entry() {}

    Entry() {
        this->type = EMPTY;
    }

    Entry(string* s, TYPE t) {
        this->str = s;
        this->type = t;
    }

    Entry(const int val, TYPE t) {
        this->i = val;
        this-> f = (float)val;
        this->str = new string(to_string(val));
        this-> type = t;
    }

    Entry(const bool bl, TYPE t) {
        this->bl = bl;
        if (bl) {
            this->i = 1;
            this->f = 1;
        } else {
            this->i = 0;
            this->f = 0;
        }
        this->str = new string(to_string(bl));
        this->type = t;
    }

    Entry(const float f, TYPE t) {
        this->f = f;
        this->str = new string(to_string(f));
        this->type = t;
    }

    Entry(Entry * e) {
        str = e->str;
        bl = e->bl;
        i = e->i;
        f = e->f;
        type = e->type; 
    }

    void changeType(TYPE t) {
        this->type = t;
    }

    string* print() {
        string* ret;
        switch(type) {
            case INT: {
                ret = new string(to_string(i));
                return ret;
            }
            case BOOL: {
                ret = new string(to_string(bl));
                return ret;
            }
            case FLOAT: {
                ret = new string(to_string(f));
                return ret;
            }
            case STRING: {
                return str;
            }
            case EMPTY: {
                ret = new string();
                return ret;
            }
            default:
                ret = new string();
                return ret;
        }

    }

    int getIntValue() {
        return this->i;
    }

    string* getStringValue() {
        return this->str;
    }

    float getFloatValue() {
        return this->f;
    }

    bool getBoolValue() {
        return this->bl;
    }


};