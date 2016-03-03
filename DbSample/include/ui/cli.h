#pragma once

#include "client.h"
#include "db_facade.h"

namespace notes {
namespace ui {

class CliClient : public Client {

    enum class CliState { MAIN, NOTEBOOKS, NOTES, TAGS };

  public:
    CliClient(std::shared_ptr<db::NotebookDatabase> &db, int argc, char **args);
    virtual ~CliClient();

    virtual int run();

  private:
    void printMenu();
    // notebook ID of current notebook, or -1 for exit
    int processInput();

    void showMainView();
    void showNotes();
    void showTags();
    void showNotebooks();

    int openNotebook();
    void listNotebooks();
    void addNote();
    void deleteNote();
    void listNotes();
    void searchNotes();
    void printNote(const notes::model::Note &);

    constexpr static int kDefaultNotebook = 0;

    int current_notebook_ = kDefaultNotebook;
    std::shared_ptr<db::NotebookDatabase> db_;
    CliState state_;
    int argc_;
    char **args_;
};

} // ui
} // notes
