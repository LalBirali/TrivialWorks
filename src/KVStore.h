#pragma once
#include <vector>
#include "dataframe.h"
#include "key.h"


/**
KVStore::
A KVStore maps a Key to a Dataframe (will be changed to a serialized dataframe in the future).
A Key is represented by a name and node and is mapped to a dataframe associated with that node.
All of the dataframes combined sum to the main dataframe that was in the sorer file.
*/
class KVStore {
    public:
    std::vector<Key*> keys;
    std::vector<DataFrame*> dfs;
    /**
    Construtor for KVStore
    */
    KVStore() {}

    /**
    Destructs a Dataframe
    */
    ~KVStore() {}

    /**
    This method returns the dataframe associated with the given key.
    */
    DataFrame * get(Key * k) {
        if (k == NULL) {
            return nullptr;
        }
        for (unsigned int i = 0; i < keys.size(); i++) {
            if (k->equals(keys[i])) {
                return dfs[i];
            }
        }
        return nullptr;
    }

    /**
    Returns the dataframe assciated with the given key.
    */
    DataFrame * waitAndGet(Key & k) {
        return get(&k);
    }
    
    /**
    Maps the inputted dataframe to the given key.
    If the key has been mapped to a different dataframe, that dataframe is returned.
    If not, a nullptr is returned.
    */
    DataFrame * put(Key * k, DataFrame * df) {
        if (k == NULL || df == NULL) {
            return nullptr;
        }
        if (get(k) == nullptr) {
            keys.push_back(k);
            dfs.push_back(df);
            return nullptr;
        } else {
            for (unsigned int i = 0; i < keys.size(); i++) {
                if (k->equals(keys[i])) {
                    DataFrame * temp = dfs[i];
                    dfs[i] = df;
                    return temp;
                }
            }
        }
        return nullptr;
    }
};