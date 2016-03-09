#include "ui/cli.h"

#include <iostream>
#include <iomanip>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace notes
{
namespace ui
{

using namespace std;
namespace pt = boost::posix_time;

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

void CliClient::printSep(char c, size_t width)
{
    std::string sep(width, c);
    std::cout << sep << endl;
}

void CliClient::printHeader()
{
    cout << " ====== Notes Main Menu ======" << endl;
}

void CliClient::showMainView()
{
    cout << endl;
    printSep('-', kCliWidth);
    printHeader();
    printSep('-', kCliWidth);
    cout << endl;
    showNotebooks();
}

void CliClient::showNotes(bool withDetails)
{
    auto notes = db_->loadNotesFromNotebook(current_notebook_);
    for (const auto &note : notes) {
        cout << setw(5);
        cout << note.id() << " | " << note.title() << endl;
        if (withDetails) {
            printSep('-', kCliWidth);
            cout << note.content() << endl;
            if (note.reminder() != pt::ptime()) {
                cout << "Reminder: " << note.reminder() << endl;
            }
            cout << "Last change: " << note.lastChanged() << endl;
        }
        printSep('-', kCliWidth);
    }
}

void CliClient::showTags() {}

void CliClient::showNotebooks()
{
    auto nbs = db_->listNotebooks();
    for (const auto &nb : nbs) {
        cout << setw(5);
        cout << nb.id() << " | " << nb.title() << endl;
    }
    cout << endl;
    printSep('-', kCliWidth);
}

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
        showNotes(true);
    case CliState::TAGS:
        showTags();
    }
    processInput();

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
    cout << "->> ";
    char input;
    std::cin >> input;
    switch (input) {
    case 'o':
        openNotebook();
    case 'a':
        readNote();
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

bigint_t CliClient::openNotebook()
{
    listNotebooks();
    cout << "Select notebook id: " << endl;
    cout << ">> ";
    bigint_t notebook_id;
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

model::Note CliClient::readNote()
{
    model::Note new_note(-1, "", "", 0, pt::ptime(), pt::ptime());
    std::string tmp;

    // read title
    cout << "Enter title:" << endl << kPrompt;
    cin >> tmp;
    new_note.title(tmp);

    // read content (clear cin first)
    cin.ignore();
    cout << "Enter content: (finish with an empty line)" << endl;
    do {
        cout << kPrompt;
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
             << kPrompt;
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
    return new_note;
}

model::Notebook CliClient::readNotebook()
{
    model::Notebook nb;
    std::string tmp;

    cout << "Enter title: " << endl << kPrompt;
    cin >> tmp;
    nb.title(tmp);
    return nb;
}

model::Tag CliClient::readTag()
{
    model::Tag tag;
    std::string tmp;

    cout << "Enter title: " << endl << kPrompt;
    cin >> tmp;
    tag.title(tmp);
    return tag;
}

std::pair<bigint_t, bigint_t> CliClient::readTagAssignment()
{
    bigint_t tag_id, note_id;
    printSep('-', kCliWidth);
    cout << "Tags: ";
    showTags();
    cout << endl << "Choose tag " << kPrompt;
    cin >> tag_id;
    cout << "Notes: " << endl;
    showNotes(false);
    cout << endl << "Choose note " << kPrompt;
    cin >> note_id;
    return make_pair(tag_id, note_id);
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
