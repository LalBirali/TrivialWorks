#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "../src/object.h"
#include "../src/string.h"
#include "../src/helper.h"
#include "../src/column.h"



// g++ colserialtest.cpp -lboost_serialization
// then run ./a.out
// look at the output file - coltest
int main() {
    Sys sys;
    std::ofstream ofs("coltest");
    IntColumn* test = new IntColumn();
    StringColumn * str = new StringColumn();
    String* hello = new String("hello");
    String* world = new String("world");
    str->push_back(hello);
    str->push_back(world);
    for (int i = 0; i < 100; i++) {
        test->push_back(i+1);
    }
    {
        boost::archive::text_oarchive oa(ofs);
        oa << test;
        oa << str;
    }

    IntColumn* des;
    StringColumn* col;
    {
        std::ifstream ifs("coltest", std::ios::binary);
        if (ifs.good()) {
            boost::archive::text_iarchive ia(ifs);
            ia >> des;
            ia >> col;
        } else {
            assert(false);
        }
        
    }
    sys.t_true(des->list[86] == test->list[86]);
    sys.t_true(test->type == des->type);
    sys.t_true(test->size_ == des->size_);
    sys.t_true(str->list[0]->equals(col->list[0]));
    sys.t_true(str->type == col->type);
    sys.t_true(str->size_ == col->size_);

    return 0;
}
