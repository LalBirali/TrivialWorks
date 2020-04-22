#include <string>
#include "../src/schema.h"
#include "../src/dataframe.h"
#include "../src/column.h"
#include "../src/SorParser.h"
#include "../src/helper.h"
#include <vector>
#include "../src/key.h"


//Tests default behavior of our KV Store: 
class Test {
    public:
    Sys sys;

    Test() {}

    ~Test() {}

    /**
     * Tests that confirm that the DF is read and stored correctly upon call of the test.sor
     **/
    void testNullKey(KVStore * kv) {
        sys.t_true(kv->get(NULL) == nullptr);
        //sys.t_true(kv->waitAndGet(NULL) == nullptr);
        sys.t_true(kv->put(NULL, NULL) == nullptr);

        sys.pln("NULL Keys handled correctly -- TESTS PASS.");
    }



};

// To be used like /sorer *Docname* "Number of Nodes"
int main(int argh, char** argv) {
    int numNodes = atoi(argv[2]);
    if (argh != 3) {
        exit(1);
    }
    if (numNodes < 1) {
        exit(1);
    }
    SorParser * sor = new SorParser();
    sor->parseDocFile(argv[1], 0, 10000);
    StrBuff * stb = new StrBuff();
    for (unsigned int i = 0; i < sor->types.size(); i++) {
        switch(sor->types.at(i)) {
            case INT: {
                stb->c("I");
                break;
            }
            case BOOL: {
                stb->c("B");
                break;
            }
            case FLOAT: {
                stb->c("F");
                break;
            }
            case STRING: {
                stb->c("S");
                break;
            }
            case EMPTY: {
                stb->c("B");
                break;
            }
        
        }
    }
    char* val = stb->get()->c_str();
    Schema newSchema(val);
    DataFrame df(newSchema);
    Row r(newSchema);
    for (unsigned int i = 0; i < sor->m.size(); i++) {
        for (unsigned int j = 0; j < sor->m.at(i).size(); j++) {
            switch(sor->types.at(j)) {
                case INT: {
                    r.set(j, sor->m.at(i).at(j)->getIntValue());
                    break;
                }
                case BOOL: {
                    r.set(j, sor->m.at(i).at(j)->getBoolValue());
                    break;
                }
                case FLOAT: {
                    r.set(j, sor->m.at(i).at(j)->getFloatValue());
                    break;
                }
                case STRING: {
                    String * str = new String(sor->m.at(i).at(j)->getStringValue()->c_str());
                    r.set(j, str);
                    break;
                }
                case EMPTY: {
                    r.set(j, sor->m.at(i).at(j)->getBoolValue());
                    break;
                }
            }
        }
        df.add_row(r);
    }
    Test* t = new Test();
    
    KVStore * kv = new KVStore();
    for (int i = 0; i < numNodes; i++) {
        int x = floor((i * df.nrows())/numNodes);
        int y = floor((i+1)*df.nrows()/numNodes);
        Key * k = new Key(to_string(i), i);
        kv->put(k, df.subset(x, y));
    }
    t->testNullKey(kv);
    
    delete t;
    delete kv;
}