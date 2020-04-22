#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "../src/object.h"
#include "../src/string.h"
#include "../src/helper.h"
#include "../src/schema.h"

void testEmptySchema() {
    std::ofstream ofs("empty_schema");
    Schema* test = new Schema();
    {
        boost::archive::text_oarchive oa(ofs);
        oa << test;
    }

    Schema* des;
    {
        std::ifstream ifs("empty_schema", std::ios::binary);
        boost::archive::text_iarchive ia(ifs);
        ia >> des;
    }
    Sys sys;
    sys.t_true(test->nrow == des->nrow);
    sys.t_true(test->ncol == des->ncol);
    sys.t_true(strcmp(test->col_types, des->col_types) == 0);
    delete test;
    delete des;
}

void testNonEmptySchema() {
    std::ofstream ofs("nonempty_schema");
    Schema* test = new Schema("BFSIISFB");
    {
        boost::archive::text_oarchive oa(ofs);
        oa << test;
    }

    Schema* des;
    {
        std::ifstream ifs("nonempty_schema", std::ios::binary);
        boost::archive::text_iarchive ia(ifs);
        ia >> des;
    }
    Sys sys;
    sys.t_true(test->nrow == des->nrow);
    sys.t_true(test->ncol == des->ncol);
    sys.pln(des->col_types);
    sys.pln(test->col_types);
}





int main() {
    testEmptySchema();
    testNonEmptySchema();
    return 0;
}
