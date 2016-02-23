#include "ui/cli.h"

#include <iostream>

namespace notes {
namespace ui {

CliClient::~CliClient() {}

CliClient::CliClient(std::shared_ptr<notes::db::NotebookDatabase> &db, int argc,
                     char **args)
    : db_(db), argc_(argc), args_(args) {}

int CliClient::run() {
    bool abort = false;
    while (!abort) {
        printMenu();
        abort = processInput();
    }
    return 0;
}

void CliClient::printMenu() {
    using namespace std;
    cout << endl;
    cout << "-----------------------" << endl;
    cout << "=== Notes Main Menu ===" << endl;
    cout << "-----------------------" << endl << endl;
    cout << "Available Notebooks: " << endl;
    listNotebooks();
    cout << "-----------------------" << endl << endl;
    cout << "o: Open Notebook" << endl;
    cout << "a: Add Note" << endl;
    cout << "l: List Notes" << endl;
    cout << "s: Search Notes" << endl;
    cout << "d: Delete Note" << endl;
    cout << "q: Quit" << endl << endl;
    cout << ">> ";
}

// if true, quit the cli loop
bool CliClient::processInput() {
    char input;
    std::cin >> input;
    switch (input) {
    case 'a':
        addNote();
        break;
    case 'l':
        listNotes();
        break;
    case 's':
        searchNotes();
        break;
    case 'd':
        deleteNote();
        break;
    case 'q':
        return true;
        break;
    default:
        std::cout << "Invalid input, please try again ..." << std::endl;
    }
    return false;
}

void CliClient::listNotebooks() { auto notebooks = db_->listNotebooks(); }

void CliClient::addNote() {}

void CliClient::deleteNote() {}

void CliClient::listNotes() {
    auto notes = db_->loadNotesFromNotebook(current_notebook_);
}

void CliClient::searchNotes() {}

} // ns ui
} // ns notes
