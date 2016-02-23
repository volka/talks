#pragma once

#include "client.h"
#include "db_facade.h"

namespace notes {
namespace ui {

class CliClient : public Client {

  public:
    CliClient(std::shared_ptr<db::NotebookDatabase> &db, int argc, char **args);
    virtual ~CliClient();

    virtual int run();

  private:
    void printMenu();
    bool processInput();

    void listNotebooks();
    void addNote();
    void deleteNote();
    void listNotes();
    void searchNotes();

    constexpr static int kDefaultNotebook = 0;

    int current_notebook_ = kDefaultNotebook;
    std::shared_ptr<db::NotebookDatabase> db_;
    int argc_;
    char **args_;
};

} // ui
} // notes
