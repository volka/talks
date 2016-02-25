#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>

#include "db_facade.h"

#include "client.h"
#include "ui/cli.h"
#include "ui/qt.h"

int main(int argc, char **argv) {
    using namespace std;
    using namespace notes;

    if (argc < 4) {
        cout << endl;
        cout << "Usage: notes <INTERFACE> <DB_TYPE> \"<CONNECTION STRING>\" "
                "--demo" << endl << endl;
        cout << "  INTERFACE: cli , qt" << endl;
        cout << "  DB_TYPE  : sqlite, pg ..." << endl;
        cout << "  CONN STR : db specific, e.g. mydb.sqlite for SQLite3"
             << endl;
        cout << "    --demo : use for Demo mode to create and fill DB" 
             << endl << endl;
    } else {
        try {
            std::shared_ptr<db::NotebookDatabase> db =
                db::NotebookDatabase::create(argv[2], argv[3]);

            // demo mode - create some dummy data
            for (auto i = 0; i < argc; ++i) {
                if (strcmp(argv[i], "--demo") == 0) {
                    db->setupDb();
                    db->fillDb();
                    break;
                }
            }

            auto ui = ui::Client::create(argv[1], db, argc, argv);
            ui->run();
        } catch (std::exception &ex) {
            std::cout << "Fatal error: " << ex.what() << endl;
            exit(-1);
        }
    }
    exit(0);
}
