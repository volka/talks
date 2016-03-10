#include <cassert>
#include <iostream>
#include <memory>
#include <cstring>
#include <algorithm>

#include "db_facade.h"
#include <Wt/Dbo/Types>

using namespace notes;
using namespace std;

void test(shared_ptr<db::NotebookDatabase> db)
{
    using namespace model;
    using namespace db;

    db->setupDb();
    db->fillDb();

    cout << "Listing notebooks ..." << endl;
    auto notebooks = db->listNotebooks();
    assert(notebooks.size() == 2);
    assert(notebooks[0].title() == "Privat");
    assert(notebooks[1].title() == "Einkaufslisten");

    for (const auto &nb : notebooks) {
        cout << nb.id() << " - " << nb.title() << endl;
    }

    cout << "Listing tags ..." << endl;
    auto tags = db->listTags();
    assert(tags.size() == 1);
    assert(tags[0].title() == "C++");
    cout << tags[0].id() << " - " << tags[0].title() << endl;

    cout << "Loading notes for notebook " << notebooks[1].id() << endl;
    auto notes_ek = db->loadNotesFromNotebook(notebooks[1].id());
    assert(notes_ek.size() == 1);
    for (const auto &note : notes_ek) {
        cout << "Got note " << note.id() << " - title: " << note.title()
             << endl;
    }

    cout << "Loading notes for tag " << tags[0].id() << endl;
    auto notes_cpp = db->loadNotesForTag(tags[0].id());
    assert(notes_cpp.size() == 2);
    assert(notes_cpp[0].title() == "Vortrag");
    assert(notes_cpp[1].content() == "Code schreiben und testen");
    for (const auto &note : notes_ek) {
        cout << "Got note " << note.id() << " - title: " << note.title()
             << endl;
    }

    cout << "Load note by ID " << notes_cpp[0].id() << endl;
    // second copy of note
    Note vortrag = db->loadNote(notes_cpp[0].id());
    assert(vortrag.id() == notes_cpp[0].id());
    assert(vortrag.title() == "Vortrag");
    assert(vortrag.notebook() == notebooks[0].id()); // 'Privat' notebook
    cout << "Got note " << vortrag.id() << " - title: " << vortrag.title()
         << endl;

    cout << "Loading notebook by id " << notebooks[0].id() << endl;
    // load single notebook
    Notebook privat = db->loadNotebook(notebooks[0].id());
    assert(privat.title() == "Privat");
    cout << "Got notebook " << privat.id() << " - " << privat.title() << endl;

    cout << "Fetching notes using search ... " << endl;
    // search notes
    auto search = db->searchNotes("C++");
    assert(search.size() == 2);
    search = db->searchNotes("Eier");
    assert(search.size() == 1);
    assert(search[0].title() == "Essen");

    for (const auto &note : search) {
        cout << "Got note " << note.id() << " - title: " << note.title()
             << endl;
    }

    cout << "Updating note " << search[0].id() << endl;
    // update
    Note &food = search[0];
    food.title("Futter");
    auto new_reminder = pt::ptime(pt::time_from_string("2016-03-11 13:12:01"));
    food.reminder(new_reminder);
    db->updateNote(food);
    cout << "Update done ... reloading note" << endl;
    Note food_reload = db->loadNote(food.id());
    assert(food_reload.title() == "Futter");
    assert(food_reload.reminder() == new_reminder);

    cout << "Got note " << food_reload.id()
         << " - title: " << food_reload.title() << endl;
    cout << "Deleting tag " << tags[0].id() << endl;
    // deleting tags and notes, check foregin key constrains
    bigint_t cpp_tag_id = tags[0].id();
    try {
        db->deleteTag(cpp_tag_id);
        tags = db->listTags();
        assert(tags.size() == 0);
    } catch (notes::db::DatabaseException &ex) {
        // on PostgreSQL, this should fail and throw an exception
        // this we should never see the assert :)
        std::cout << "expected exception: " << ex.what() << endl;
    }

    cout << ".. and try to remove deleted tag from notes..." << endl;
    db->removeTag(notes_cpp[0].id(), cpp_tag_id);
    db->removeTag(notes_cpp[1].id(), cpp_tag_id);

    cout << "Now load notes by deleted tag" << endl;
    auto empty_notes = db->loadNotesForTag(cpp_tag_id);
    assert(empty_notes.empty());
    if (empty_notes.empty())
        cout << "Got empty note list as expected" << endl;

    cout << "now really remove tag if constaints prevented us from doing this "
            "earlier" << endl;
    db->deleteTag(cpp_tag_id);
    tags = db->listTags();
    assert(tags.empty());
    if (tags.empty())
        cout << "Now tag list is empty" << endl;

    cout << "Delete note 0 from 'Einkauf'" << endl;
    db->deleteNote(notes_ek[0].id());
    notes_ek = db->loadNotesFromNotebook(notebooks[1].id());
    assert(notes_ek.empty());
    cout << "Reload of notebook 'Einkauf' - now with " << notes_ek.size()
         << " notes" << endl;

    // notebook modifications
    cout << "Delete a notebook" << endl;
    db->deleteNotebook(notebooks[1].id());
    cout << "Rename the other one" << endl;
    db->renameNotebook(notebooks[0].id(), "Foo");

    cout << "List and check if that worked ..." << endl;
    notebooks = db->listNotebooks();
    assert(notebooks.size() == 1);
    assert(notebooks[0].title() == "Foo");
    cout << "Notebooks changed as expected" << endl;
    cout << "=== TEST FINISHED ===" << endl;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        cout << endl;
        cout << "Usage: notebook_test <DB_TYPE> \"<CONNECTION STRING>\" "
             << endl;
        cout << "  DB_TYPE  : sqlite, postgres, qtsql, wtdbo, sqlpp" << endl;
        cout << "  CONN STR : db specific, e.g. mydb.sqlite for SQLite3" << endl
             << endl;
    } else {
        try {
            shared_ptr<db::NotebookDatabase> db =
                db::NotebookDatabase::create(argv[1], argv[2]);
            assert(db.get() != nullptr &&
                   "create did not return valid db pointer");

            test(db);

        } catch (exception &ex) {
            cout << "Fatal error: " << ex.what() << endl;
            exit(-1);
        }
    }
    exit(0);
}
