#include "db/sqlpp_db.h"
#include <iostream>

namespace notes
{
namespace db
{

SqlppDatabase::SqlppDatabase(const std::string &connection_info)
    : connection_info_(connection_info)
{
}

SqlppDatabase::~SqlppDatabase() {}

void SqlppDatabase::setupDb() {}

std::vector<Notebook> SqlppDatabase::listNotebooks()
{
    return std::vector<Notebook>();
}

int SqlppDatabase::newNotebook(const std::string &title)
{
    std::cout << "new notebook " << title << std::endl;
    return 0;
}

void SqlppDatabase::renameNotebook(const int notebook_id,
                                   const std::string &new_title)
{
    std::cout << "rename notebook " << notebook_id << " to " << new_title
              << std::endl;
}

void SqlppDatabase::deleteNotebook(const int id)
{
    std::cout << "delete notebook " << id << std::endl;
}

Notebook SqlppDatabase::loadNotebook(const int notebook_id)
{
    return Notebook(notebook_id, "foo");
}

void SqlppDatabase::newNote(Note &) {}

void SqlppDatabase::updateNote(const Note &) {}

void SqlppDatabase::addTag(const int note_id, const int tag_id)
{
    std::cout << "add tag " << tag_id << " to " << note_id << std::endl;
}

void SqlppDatabase::removeTag(const int note_id, const int tag_id)
{
    std::cout << "remove tag " << tag_id << " to " << note_id << std::endl;
}

void SqlppDatabase::deleteNote(const int id)
{
    std::cout << "delete note " << id << std::endl;
}

Note SqlppDatabase::loadNote(const int note_id)
{
    return Note(note_id, "foo", "bar", 0, pt::ptime(), pt::ptime());
}

int SqlppDatabase::newTag(const std::string &title)
{
    std::cout << "create new tag " << title << std::endl;
    return 0;
}

std::vector<Tag> db::SqlppDatabase::listTags()
{
    return std::vector<model::Tag>();
}

void SqlppDatabase::deleteTag(const int tag_id)
{
    std::cout << "delete tag " << tag_id << std::endl;
}

std::vector<Note> SqlppDatabase::loadNotesFromNotebook(const int notebook_id)
{
    std::cout << "load notes for notebook " << notebook_id << std::endl;
    return std::vector<Note>();
}

std::vector<Note> SqlppDatabase::loadNotesForTag(const int tag_id)
{
    std::cout << "load notes for tag " << tag_id << std::endl;
    return std::vector<Note>();
}

std::vector<Note> SqlppDatabase::searchNotes(const std::string &term)
{
    std::cout << "searching in notes for " << term << std::endl;
    return std::vector<Note>();
}

} // ns db
} // ns notes
