#include <iostream>
#include <memory>

#include "db_facade.h"

#include "client.h"
#include "ui/cli.h"
#include "ui/qt.h"

int main(int argc, char **argv) {
    using namespace std;
    using namespace notes;

    if (argc < 4) {
        cout
            << "Usage: notes <INTERFACE> <DB_TYPE> \"<CONNECTION STRING>\" demo"
            << endl;
        cout << " - INTERFACE: cli , qt" << endl;
        cout << " - DB_TYPE  : sqlite, pg ..." << endl;
        cout << " - CONN STR : db specific, e.g. mydb.sqlite for SQLite3"
             << endl;
        cout << " - demo : use for Demo mode to create and fill DB" << endl;
    } else {
        try {
            std::shared_ptr<db::NotebookDatabase> db =
                db::NotebookDatabase::create(argv[2], argv[3]);
            if (argc > 4 && std::string("demo") == argv[4]) {
                db->setupDb();
                db->fillDb();
            }
            auto ui = ui::Client::create(argv[1], db);
            ui->run();
        } catch (std::exception &ex) {
            std::cout << "Fatal error: " << ex.what() << endl;
            exit(-1);
        }
    }
    exit(0);
}
