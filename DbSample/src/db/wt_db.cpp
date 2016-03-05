#include "db/wt_db.h"
#include "db_facade.h"

namespace notes
{

db::WtDatabase::WtDatabase(const std::string &conn_info)
{
    config_ = db::parseConnectionInfo(conn_info);
    // TODO : actually connect :)
}

db::WtDatabase::~WtDatabase() {}

void db::WtDatabase::setupDb() {}

void db::WtDatabase::fillDb() {}

std::vector<model::Notebook> db::WtDatabase::listNotebooks()
{
    return std::vector<model::Notebook>();
}

int db::WtDatabase::newNotebook(const std::string &title)
{
    wt::Notebook n;
    n.title = title;
    return 0;
}

void db::WtDatabase::renameNotebook(const int notebook_id,
                                    const std::string &new_title)
{
    wt::Notebook n;
    n.id = notebook_id;
    n.title = new_title;
}

void db::WtDatabase::deleteNotebook(const int id)
{
    wt::Notebook n;
    n.id = id;
}

model::Notebook db::WtDatabase::loadNotebook(const int notebook_id)
{
    wt::Notebook n;
    n.id = notebook_id;
    return model::Notebook();
}

void db::WtDatabase::newNote(model::Note &) {}

void db::WtDatabase::updateNote(const model::Note &) {}

void db::WtDatabase::addTag(const int note_id, const int tag_id)
{
    wt::Note n;
    n.id = note_id;
    wt::Tag t;
    t.id = tag_id;
}

void db::WtDatabase::removeTag(const int note_id, const int tag_id)
{
    wt::Note n;
    n.id = note_id;
    wt::Tag t;
    t.id = tag_id;
}

void db::WtDatabase::deleteNote(int id)
{
    wt::Note n;
    n.id = id;
}

model::Note db::WtDatabase::loadNote(int note_id)
{
    wt::Note n;
    n.id = note_id;
    return model::Note();
}

int db::WtDatabase::newTag(const std::string &title)
{
    wt::Tag t;
    t.title = title;
    return 0;
}

int db::WtDatabase::findTag(const std::string &title)
{
    wt::Tag t;
    t.title = title;
    return 0;
}

void db::WtDatabase::deleteTag(const int tag_id)
{
    wt::Tag t;
    t.id = tag_id;
}

std::vector<model::Note> db::WtDatabase::loadNotesFromNotebook(int notebook_id)
{
    wt::Notebook nb;
    nb.id = notebook_id;
    return std::vector<model::Note>();
}

std::vector<model::Note> db::WtDatabase::loadNotesForTag(int tag_id)
{
    wt::Tag t;
    t.id = tag_id;
    return std::vector<model::Note>();
}

} // ns notes
