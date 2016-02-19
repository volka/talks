#include <iostream>
#include <memory>

#include "db_facade.h"

#include "client.h"
#include "ui/cli.h"
#include "ui/qt.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << "Usage: notes <INTERFACE> <DB_TYPE> \"<CONNECTION STRING>\""
              << std::endl;
    std::cout << " - INTERFACE: cli , qt" << std::endl;
    std::cout << " - DB_TYPE  : sqlite, pg ..." << std::endl;
    std::cout << " - CONN STR : db specific, e.g. mydb.sqlite for SQLite3"
              << endl;
  } else {
    auto db = notes::db::connect(argv[2], argv[3]);
    std::unique_ptr<Client> ui;
    if (strcmp(argv[1], "cli") == 0) {
      ui = std::make_unique<notes::ui::CliClient>(db.get());
    } else if (strcmp(argv[1], "qt") == 0) {
      ui = std::make_unique<notes::ui::QtClient>(db.get());
    }
    ui->run();
  }
}
