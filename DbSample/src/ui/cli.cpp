#include "ui/cli.h"

#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace notes {
namespace ui {

using namespace std;

CliClient::~CliClient() {}

CliClient::CliClient(std::shared_ptr<notes::db::NotebookDatabase> &db, int argc,
                     char **args)
    : db_(db), argc_(argc), args_(args) {}

int CliClient::run() {
    do {
        printMainMenu();
    } while (!processMainMenuInput());
    return 0;
}

void CliClient::printMainMenu() {
    using namespace std;
    cout << endl;
    cout << "-----------------------" << endl;
    cout << "=== Notes Main Menu ===" << endl;
    cout << "-----------------------" << endl << endl;

    listNotes();

    cout << "-----------------------" << endl << endl;
    cout << "o: Open Notebook" << endl;
    cout << "a: Add Note" << endl;
    cout << "e: Edit Note" << endl;
    cout << "d: Delete Note" << endl;
    cout << "l: List by tag" << endl;
    cout << "q: Quit" << endl << endl;
    cout << ">> ";
}

// if true, quit the cli loop
int CliClient::processMainMenuInput() {
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
    cout << "Found " << notebooks.size() << " notebooks:" << endl;
    for (const auto &notebook : notebooks) {
        cout << notebook.id() << ": " << notebook.title() << endl;
    }
}

void CliClient::addNote() {
    model::Note new_note;
    std::string tmp;
    cout << "Enter title:" << endl << ">> ";
    cin >> tmp;
    new_note.title(tmp);
    cout << "Enter content: (finish with a line just containing '.')" << endl;
    while (tmp != ".") {
        cout << "-- >>";
        cin >> tmp;
        new_note.content( new_note.content() + tmp + "\n");
    }

    namespace pt = boost::posix_time;
    pt::ptime time_tmp;
    cout << "Enter reminder date: (yyyy-mm-dd hh:mm:ss)" << endl << ">> ";
    cin >> tmp;
    //time_tmp = pt::time_from_string(tmp);
    new_note.reminder(time_tmp);

    new_note.lastChanged(pt::second_clock::local_time());
    cout << "Inserting Note into DB: " << endl;
    printNote(new_note);
    db_->newNote(new_note);
}

void CliClient::deleteNote() {}

void CliClient::listNotes() {
    auto notes = db_->loadNotesFromNotebook(current_notebook_);
    cout << "Found " << notes.size() << " notes: " << endl;
    for (const auto &note : notes) {
        printNote(note);
    }
    cout << "-----------------------" << endl << endl;
}

void CliClient::printNote(const notes::model::Note &note) {
    cout << "ID      : " << note.id() << endl;
    cout << "Title   : " << note.title() << endl;
    cout << "Changed : " << note.lastChanged() << endl;
    cout << "Reminder: " << note.reminder() << endl;
    cout << note.content() << endl;
    cout << "-----------------------" << endl << endl;
}

} // ns ui
} // ns notes
