#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "../src/object.h"
#include "../src/string.h"
#include "../src/helper.h"
#include "../src/schema.h"
#include "../src/row.h"
#include "../src/dataframe.h"

int main() {
    std::ofstream ofs("dftest");
    Schema* test = new Schema("I");
    DataFrame * df = new DataFrame(*test);
    Row r(df->get_schema());

    for (int i = 1; i <= 10; i++) {
        r.set(0, i);
        df->add_row(r);
    }
    {
        boost::archive::text_oarchive oa(ofs);
        oa << df;
    }

    DataFrame* des;
    {
        std::ifstream ifs("dftest", std::ios::binary);
        boost::archive::text_iarchive ia(ifs);
        ia >> des;
    }
    Sys sys;
    sys.t_true(des->ncol == df->ncol);
    sys.t_true(des->nrow == df->nrow);
    Column* c = des->columns[0];
    sys.pln(c->type);
    return 0;
}
