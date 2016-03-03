#include "db/wt_db.h"

using namespace notes;

void db::WtDatabase::setupDb()
{
}

void db::WtDatabase::fillDb()
{
}

std::vector<model::Notebook> db::WtDatabase::listNotebooks()
{
    return std::vector<model::Notebook>();
}

int db::WtDatabase::newNotebook(const std::string &title)
{
    return 0;
}

void db::WtDatabase::renameNotebook(const int notebook_id, const std::string &new_title)
{
}

void db::WtDatabase::deleteNotebook(const int id)
{
}

model::Notebook db::WtDatabase::loadNotebook(const int notebook_id)
{
    return model::Notebook();
}

void db::WtDatabase::newNote(model::Note &)
{
}

void db::WtDatabase::updateNote(const model::Note &)
{
}

void db::WtDatabase::addTag(const int note_id, const int tag_id)
{
}

void db::WtDatabase::removeTag(const int note_id, const int tag_id)
{
}

void db::WtDatabase::deleteNote(int id)
{
}

model::Note db::WtDatabase::loadNote(int note_id)
{
    return model::Note();
}

int db::WtDatabase::newTag(const std::string &title)
{
    return 0;
}

int db::WtDatabase::findTag(const std::string &title)
{
}

void db::WtDatabase::deleteTag(const int tag_id)
{
}

std::vector<model::Note> db::WtDatabase::loadNotesFromNotebook(int notebook_id)
{
    return std::vector<model::Note>();
}

std::vector<model::Note> db::WtDatabase::loadNotesForTag(int tag_id)
{
    return std::vector<model::Note>();
}
