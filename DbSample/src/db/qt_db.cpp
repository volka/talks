#include "qtsql/qt_db.h"

#include <iostream>

namespace notes {
namespace db {

using namespace model;

void QtDatabase::setupDb()
{
}

void QtDatabase::fillDb()
{
}

std::vector<Notebook> QtDatabase::listNotebooks()
{
    return std::vector<Notebook>{};
}

int QtDatabase::newNotebook(const std::string &title)
{
    std::cout << title << std::endl;
    return 0;
}

void QtDatabase::renameNotebook(const int notebook_id, const std::string &new_title)
{
    std::cout << notebook_id << new_title << std::endl;
    return;
}

void QtDatabase::deleteNotebook(const int id)
{
    std::cout << id << std::endl;
    return;
}

Notebook QtDatabase::loadNotebook(const int notebook_id)
{
    std::cout << notebook_id << std::endl;
    return Notebook{};
}

void QtDatabase::newNote(Note &)
{
    return;
}

void QtDatabase::updateNote(const Note &)
{
    return;
}

void QtDatabase::addTag(const int note_id, const int tag_id)
{
    std::cout << "addTag: " << note_id << " " << tag_id << std::endl;
    return;
}

void QtDatabase::removeTag(const int note_id, const int tag_id)
{
    std::cout << "removeTag: " << note_id << " " << tag_id << std::endl;
    return;
}

void QtDatabase::deleteNote(int id)
{
    std::cout << "deleteNote " << id << std::endl;
    return;
}

Note QtDatabase::loadNote(int note_id)
{
    std::cout << "loadNote " << note_id << std::endl;
    return Note{};
}

int QtDatabase::newTag(const std::string &title)
{
    std::cout << "newTag " << title << std::endl;
    return 0;
}

int QtDatabase::findTag(const std::string &title)
{
    std::cout << "findTag " << title << std::endl;
    return 0;
}

void QtDatabase::deleteTag(const int tag_id)
{
    std::cout << "deleteTag" << tag_id << std::endl;
    return;
}

std::vector<Note> QtDatabase::loadNotesFromNotebook(int notebook_id)
{
    std::cout << "loadNotesFromNotebook " << notebook_id << std::endl;
    return std::vector<Note>{};
}

std::vector<Note> QtDatabase::loadNotesForTag(int tag_id)
{
    std::cout << "loadNotesForTag " << tag_id << std::endl;
    return std::vector<Note>{};
}

} // ns db
} // ns notes
