#include "db/wt_db.h"
#include "db_facade.h"

#include <memory>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Postgres>
#include <Wt/Dbo/backend/Sqlite3>

namespace notes
{

namespace dbo = Wt::Dbo;

db::WtDatabase::WtDatabase(const std::string &conn_info)
{
    // first create a connection and set it on the session_ object
    config_ = db::parseConnectionInfo(conn_info);
    if (config_.driver == "SQLITE") {
        dbo::backend::Sqlite3 sqlite3(config_.dbname);
        session_.setConnection(sqlite3);
    } else if (config_.driver == "PG") {
        dbo::backend::Postgres pg(config_.conn_str);
        session_.setConnection(pg);
    }

    session_.mapClass<wt::Note>("notes");
    session_.mapClass<wt::Notebook>("notebooks");
    session_.mapClass<wt::Tag>("tags");
}

db::WtDatabase::~WtDatabase() {}

void db::WtDatabase::setupDb()
{
    session_.dropTables();
    session_.createTables();
}

// listing per find - WARNING: result is sliced !
std::vector<model::Notebook> db::WtDatabase::listNotebooks()
{
    std::vector<model::Notebook> result;

    dbo::collection<dbo::ptr<Notebook>> nb_list = session_.find<Notebook>();
    for (const auto &nb : nb_list) {
        result.emplace_back(*nb.get());
    }
    return result;
}

// we can use unique_ptrs for resource management before adding values to
// session_
int db::WtDatabase::newNotebook(const std::string &title)
{
    dbo::Transaction t(session_);
    auto nb = std::make_unique<wt::Notebook>(title);
    dbo::ptr<Notebook> nb_ptr = session_.add(nb.release());
    t.commit();
    return nb_ptr.get()->id();
}

// find() / modify() example
void db::WtDatabase::renameNotebook(const int notebook_id,
                                    const std::string &new_title)
{
    dbo::Transaction t(session_);
    dbo::ptr<Notebook> nb =
        session_.find<Notebook>().where("id=?").bind(notebook_id);
    nb.modify()->title(new_title);
    t.commit();
}

void db::WtDatabase::deleteNotebook(const int id)
{
    dbo::Transaction t(session_);
    dbo::ptr<Notebook> nb = session_.find<Notebook>().where("id=?").bind(id);
    nb.remove();
    t.commit();
}

model::Notebook db::WtDatabase::loadNotebook(const int notebook_id)
{
    dbo::ptr<Notebook> nb =
        session_.find<Notebook>().where("id=?").bind(notebook_id);
    return *(nb.modify());
}

void db::WtDatabase::newNote(model::Note &note)
{
    dbo::Transaction t(session_);
    auto db_note = std::make_unique<wt::Note>(note);
    session_.add(db_note.release());
    t.commit();
}

void db::WtDatabase::updateNote(const model::Note &note)
{
    dbo::Transaction t(session_);
    dbo::ptr<Note> db_note =
        session_.find<Note>().where("id=?").bind(note.id());
    *(db_note.modify()) = note;
    t.commit();
}

void db::WtDatabase::addTag(const int note_id, const int tag_id)
{
    wt::Note n;
    n.id(note_id);
    wt::Tag t;
    t.id(tag_id);
}

void db::WtDatabase::removeTag(const int note_id, const int tag_id)
{
    wt::Note n;
    n.id(note_id);
    wt::Tag t;
    t.id(tag_id);
}

void db::WtDatabase::deleteNote(const int note_id)
{
    wt::Note n;
    n.id(note_id);
}

model::Note db::WtDatabase::loadNote(const int note_id)
{
    wt::Note n;
    n.id(note_id);
    return model::Note(-1, "foo", "bar", 0, pt::ptime(), pt::ptime());
}

int db::WtDatabase::newTag(const std::string &title)
{
    wt::Tag t;
    t.title(title);
    return 0;
}

std::vector<model::Tag> db::WtDatabase::listTags()
{
    return std::vector<model::Tag>();
}

void db::WtDatabase::deleteTag(const int tag_id)
{
    wt::Tag t;
    t.id(tag_id);
}

std::vector<model::Note>
db::WtDatabase::loadNotesFromNotebook(const int notebook_id)
{
    wt::Notebook nb;
    nb.id(notebook_id);
    return std::vector<model::Note>();
}

std::vector<model::Note> db::WtDatabase::loadNotesForTag(const int tag_id)
{
    wt::Tag t;
    t.id(tag_id);
    return std::vector<model::Note>();
}

std::vector<model::Note> db::WtDatabase::searchNotes(const std::string &term)
{
    std::cout << "searching in notes for " << term << std::endl;
    return std::vector<model::Note>();
}

} // ns notes
