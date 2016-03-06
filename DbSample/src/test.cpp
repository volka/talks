#include <cassert>
#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>

#include "db_facade.h"

using namespace notes;
using namespace std;

void test(shared_ptr<db::NotebookDatabase> db)
{
    db->setupDb();
    db->fillDb();
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        cout << endl;
        cout << "Usage: notebook_test <DB_TYPE> \"<CONNECTION STRING>\" "
             << endl;
        cout << "  DB_TYPE  : sqlite, postgres, qtsql, wtdbo, sqlpp" << endl;
        cout << "  CONN STR : db specific, e.g. mydb.sqlite for SQLite3" << endl
             << endl;
    } else {
        try {
            shared_ptr<db::NotebookDatabase> db =
                db::NotebookDatabase::create(argv[2], argv[3]);
            assert(db.get() != nullptr &&
                   "create did not return valid db pointer");

            test(db);

        } catch (exception &ex) {
            cout << "Fatal error: " << ex.what() << endl;
            exit(-1);
        }
    }
    exit(0);
}
