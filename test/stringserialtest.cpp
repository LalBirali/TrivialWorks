#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "../src/object.h"
#include "../src/string.h"
#include "../src/helper.h"

int main() {
    std::ofstream ofs("name");
    String* test = new String("test");
    {
        boost::archive::text_oarchive oa(ofs);
        oa << test;
    }

    String* des;
    {
        std::ifstream ifs("name", std::ios::binary);
        boost::archive::text_iarchive ia(ifs);
        ia >> des;
    }
    Sys sys;
    sys.t_true(test->equals(des));
    return 0;
}