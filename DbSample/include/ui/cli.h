#pragma once

#include "client.h"
#include "db_facade.h"

namespace notes
{
namespace ui
{

using bigint_t = long long;

class CliClient : public Client
{

    enum class CliState { MAIN, NOTEBOOKS, NOTES, TAGS };

  public:
    CliClient(std::shared_ptr<db::NotebookDatabase> &db, int argc, char **args);
    virtual ~CliClient();

    virtual int run();

  private:
    void printMenu();
    // notebook ID of current notebook, or -1 for exit
    int processInput();

    // "windows" for each state
    void showMainView();
    void showNotebookView();


    // display text of model classes
    void showNotes(bool withDetails);
    void showTags();
    void showNotebooks();

    bigint_t openNotebook();
    void listNotebooks();
    model::Note readNote();
    model::Notebook readNotebook();
    model::Tag readTag();
    std::pair<bigint_t, bigint_t> readTagAssignment();

    void deleteNote();
    void listNotes();
    void searchNotes();
    void printNote(const notes::model::Note &);

    constexpr static const char *kPrompt = "->> ";
    constexpr static int kDefaultNotebook = 0;
    constexpr static int kCliWidth = 30;

    bigint_t current_notebook_ = kDefaultNotebook;
    std::shared_ptr<db::NotebookDatabase> db_;
    CliState state_;
    int argc_;
    char **args_;
    void printSep(char c, size_t width);
    void printHeader();
};

} // ui
} // notes
