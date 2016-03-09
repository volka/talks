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

    auto notebooks = db->listNotebooks();
    assert(notebooks.size() == 2);
    assert(notebooks[0].title() == "Privat");
    assert(notebooks[1].title() == "Einkaufslisten");

    auto tags = db->listTags();
    assert(tags.size() == 1);
    assert(tags[0].title() == "C++");

    auto notes_ek = db->loadNotesFromNotebook(notebooks[1].id());
    assert(notes_ek.size() == 1);

    auto notes_cpp = db->loadNotesForTag(tags[0].id());
    assert(notes_cpp.size() == 2);
    assert(notes_cpp[0].title() == "Vortrag");
    assert(notes_cpp[1].content() == "Code schreiben und testen");

    // second copy of note
    Note vortrag = db->loadNote(notes_cpp[0].id());
    assert(vortrag.id() == notes_cpp[0].id());
    assert(vortrag.title() == "Vortrag");
    assert(vortrag.notebook() == notebooks[0].id()); // 'Privat' notebook

    // load single notebook
    Notebook privat = db->loadNotebook(notebooks[0].id());
    assert(privat.title() == "Privat");

    // search notes
    auto search = db->searchNotes("C++");
    assert(search.size() == 2);
    search = db->searchNotes("Eier");
    assert(search.size() == 1);
    assert(search[0].title() == "Essen");

    // update
    Note &food = search[0];
    food.title("Futter");
    auto new_reminder = pt::ptime(pt::time_from_string("2016-03-11 13:12:01"));
    food.reminder(new_reminder);
    db->updateNote(food);
    Note food_reload = db->loadNote(food.id());
    assert(food_reload.title() == "Futter");
    cout << "reminder: " << food_reload.reminder() << std::endl;
    assert(food_reload.reminder() == new_reminder);

    // deleting tags and notes, check foregin key constrains
    int cpp_tag_id = tags[0].id();
    try {
        db->deleteTag(cpp_tag_id);
        tags = db->listTags();
        assert(tags.size() == 0);
    } catch (notes::db::DatabaseException &ex) {
        // on PostgreSQL, this should fail and throw an exception
        // this we should never see the assert :)
        std::cout << "expected exception: " << ex.what() << endl;
    }

    db->removeTag(notes_cpp[0].id(), cpp_tag_id);
    db->removeTag(notes_cpp[1].id(), cpp_tag_id);

    auto empty_notes = db->loadNotesForTag(cpp_tag_id);
    assert(empty_notes.empty());

    db->deleteTag(cpp_tag_id);
    tags = db->listTags();
    assert(tags.size() == 0);

    db->deleteNote(notes_ek[0].id());
    notes_ek = db->loadNotesFromNotebook(notebooks[1].id());
    assert(notes_ek.empty());

    // notebook modifications
    db->deleteNotebook(notebooks[1].id());
    db->renameNotebook(notebooks[0].id(), "Foo");

    notebooks = db->listNotebooks();
    assert(notebooks.size() == 1);
    assert(notebooks[0].title() == "Foo");
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
