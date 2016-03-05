#include "ui/cli.h"

#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace notes
{
namespace ui
{

using namespace std;

CliClient::~CliClient() {}

CliClient::CliClient(std::shared_ptr<notes::db::NotebookDatabase> &db, int argc,
                     char **args)
    : db_(db), state_(CliState::MAIN), argc_(argc), args_(args)
{
}

int CliClient::run()
{
    do {
        printMenu();
    } while (!processInput());
    return 0;
}

void CliClient::showMainView() {}

void CliClient::showNotes() {}

void CliClient::showTags() {}

void CliClient::showNotebooks() {}

void CliClient::printMenu()
{
    using namespace std;

    switch (state_) {
    case CliState::MAIN:
        showMainView();
        break;
    case CliState::NOTEBOOKS:
        showNotebooks();
    case CliState::NOTES:
        showNotes();
    case CliState::TAGS:
        showTags();
    }
    processInput();

    cout << endl;
    cout << "-----------------------" << endl;
    cout << "=== Notes Main Menu ===" << endl;
    cout << "-----------------------" << endl << endl;

    listNotebooks();
    // listNotes();

    cout << "-----------------------" << endl << endl;
    cout << "o: Open Notebook" << endl;
    cout << "n: New Notebook" << endl;
    cout << "e: Edit current notebook" << endl;
    cout << "q: Quit" << endl;

    // TODO: move to note screen
    cout << "a: Add Note" << endl;
    cout << "e: Edit Note" << endl;
    cout << "d: Delete Note" << endl;
    cout << "l: List by tag" << endl;
    cout << "q: Quit" << endl << endl;
    cout << ">> ";
}

// if true, quit the cli loop
int CliClient::processInput()
{
    // TODO : handle state
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

int CliClient::openNotebook()
{
    listNotebooks();
    cout << "Select notebook id: " << endl;
    cout << ">> ";
    int notebook_id;
    cin >> notebook_id;
    return notebook_id;
}

void CliClient::listNotebooks()
{
    auto notebooks = db_->listNotebooks();
    cout << "Found " << notebooks.size() << " notebooks:" << endl;
    for (const auto &notebook : notebooks) {
        cout << notebook.id() << ": " << notebook.title() << endl;
    }
}

void CliClient::addNote()
{
    model::Note new_note;
    std::string tmp;

    // read title
    cout << "Enter title:" << endl << ">> ";
    cin >> tmp;
    new_note.title(tmp);

    // read content (clear cin first)
    cin.ignore();
    cout << "Enter content: (finish with an empty line)" << endl;
    do {
        cout << "->>";
        getline(cin, tmp);
        if (!tmp.empty())
            new_note.content(new_note.content() + tmp + "\n");
    } while (!tmp.empty());

    // read timestamp
    namespace pt = boost::posix_time;
    pt::ptime time_tmp;
    bool parse_ok = false;
    while (!parse_ok) {
        cout << "Enter reminder date: (yyyy-mm-dd hh:mm:ss) or 0 to skip"
             << endl
             << ">> ";
        cin >> tmp;
        try {
            if (tmp.size() > 0 && tmp[0] == '0')
                time_tmp = pt::ptime();
            else
                time_tmp = pt::time_from_string(tmp);
            parse_ok = true;

        } catch (std::exception &ex) {
            cout << "!!! error: date format is invalid, try again (or enter 0 "
                    "to skip)"
                 << endl;
        }
    }
    new_note.reminder(time_tmp);

    // last change is always now
    new_note.lastChanged(pt::second_clock::local_time());
    cout << "Inserting Note into DB: " << endl;
    printNote(new_note);
    db_->newNote(new_note);
}

void CliClient::deleteNote() {}

void CliClient::listNotes()
{
    auto notes = db_->loadNotesFromNotebook(current_notebook_);
    cout << "Found " << notes.size() << " notes: " << endl;
    for (const auto &note : notes) {
        printNote(note);
    }
    cout << "-----------------------" << endl << endl;
}

void CliClient::printNote(const notes::model::Note &note)
{
    cout << "ID      : " << note.id() << endl;
    cout << "Title   : " << note.title() << endl;
    cout << "Changed : " << note.lastChanged() << endl;
    cout << "Reminder: " << note.reminder() << endl;
    cout << note.content() << endl;
    cout << "-----------------------" << endl << endl;
}

} // ns ui
} // ns notes
