#pragma once

#include "client.h"
#include "db_facade.h"

namespace notes {
namespace ui {

class CliClient : public Client {

  public:
    CliClient(std::shared_ptr<db::NotebookDatabase> &db);
    virtual ~CliClient();

    virtual void run();

  private:
    void printMenu();
    bool processInput();

    void addNote();
    void deleteNote();
    void listNotes();
    void searchNotes();

    std::shared_ptr<db::NotebookDatabase> db_;
};

} // ui
} // notes
