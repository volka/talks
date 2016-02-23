#include "ui/cli.h"

#include <iostream>

namespace notes {
namespace ui {

using namespace std;

CliClient::~CliClient() {}

CliClient::CliClient(std::shared_ptr<notes::db::NotebookDatabase> &db, int argc,
                     char **args)
    : db_(db), argc_(argc), args_(args) {}

int CliClient::run() {
    int current_notebook = 0;
    while (current_notebook >= 0) {
        printMenu();
        current_notebook = processInput();
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
int CliClient::processInput() {
    char input;
    std::cin >> input;
    switch (input) {
    case 'o':
        openNotebook();
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

int CliClient::openNotebook() {
    listNotebooks();
    cout << "Select notebook id: " << endl;
    cout << ">> ";
    int notebook_id;
    cin >> notebook_id;
    return notebook_id;
}

void CliClient::listNotebooks() {
    auto notebooks = db_->listNotebooks();
    cout << "Found " << notebooks.size() << " noteoboks:" << endl;
    cout << "-----------------------" << endl << endl;
    for (const auto &notebook : notebooks) {
        cout << notebook.id() << ": " << notebook.title() << endl;
    }
    cout << "-----------------------" << endl << endl;
}

void CliClient::addNote() {}

void CliClient::deleteNote() {}

void CliClient::listNotes() {
    auto notes = db_->loadNotesFromNotebook(current_notebook_);
    cout << "Found " << notes.size() << " notes: " << endl;
    cout << "-----------------------" << endl << endl;
    for (const auto &note : notes) {
        printNote(note);
    }
    cout << "-----------------------" << endl << endl;
}

void CliClient::searchNotes() {}

void CliClient::printNote(const notes::model::Note &note) {
    cout << "ID      : " << note.id() << endl;
    cout << "Title   : " << note.title() << endl;
    cout << "Changed : " << note.lastChanged() << endl;
    cout << "Reminder: " << note.reminder() << endl;
    cout << "......................." << endl;
    cout << note.content() << endl;
    cout << "-----------------------" << endl << endl;
}

} // ns ui
} // ns notes
