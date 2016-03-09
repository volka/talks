#include "db/wt_db.h"
#include "db_facade.h"

#include <memory>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Postgres>
#include <Wt/Dbo/backend/Sqlite3>

namespace notes
{

using namespace db;
namespace dbo = Wt::Dbo;

WtDatabase::WtDatabase(const std::string &conn_info)
{
    // first create a connection and set it on the session_ object
    config_ = parseConnectionInfo(conn_info);
    if (config_.driver == "SQLITE") {
        dbo::backend::Sqlite3 sqlite3(config_.dbname);
        sqlite3.setDateTimeStorage(
            dbo::SqlDateTimeType::SqlDateTime,
            dbo::backend::Sqlite3::DateTimeStorage::ISO8601AsText);
        session_.setConnection(sqlite3);
    } else if (config_.driver == "PG") {
        dbo::backend::Postgres pg(config_.conn_str);
        session_.setConnection(pg);
    } else {
        throw DatabaseException("Invalid connection string, driver:" +
                                config_.driver);
    }

    session_.mapClass<wt::Note>("notes");
    session_.mapClass<wt::Notebook>("notebooks");
    session_.mapClass<wt::Tag>("tags");
}

WtDatabase::~WtDatabase() {}

void WtDatabase::setupDb()
{
    /*    dbo::Transaction t(session_);
        session_.dropTables();
        session_.createTables();
        t.commit(); */
}

// listing per find - WARNING: result is sliced !
std::vector<model::Notebook> WtDatabase::listNotebooks()
{
    std::vector<model::Notebook> result;

    dbo::collection<dbo::ptr<wt::Notebook>> nb_list =
        session_.find<wt::Notebook>();
    for (const auto &nb : nb_list) {
        result.emplace_back(*nb.get());
    }
    return result;
}

// we can use unique_ptrs for resource management before adding values to
// session_
int WtDatabase::newNotebook(const std::string &title)
{
    dbo::Transaction t(session_);
    auto nb = std::make_unique<wt::Notebook>(title);
    dbo::ptr<wt::Notebook> nb_ptr = session_.add(nb.release());
    t.commit();
    return nb_ptr->id();
}

// find() / modify() example
void WtDatabase::renameNotebook(const int notebook_id,
                                const std::string &new_title)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Notebook> nb =
        session_.find<wt::Notebook>().where("id=?").bind(notebook_id);
    nb.modify()->title(new_title);
    t.commit();
}

void WtDatabase::deleteNotebook(const int id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Notebook> nb =
        session_.find<wt::Notebook>().where("id=?").bind(id);
    nb.remove();
    t.commit();
}

model::Notebook WtDatabase::loadNotebook(const int notebook_id)
{
    dbo::ptr<wt::Notebook> nb =
        session_.find<wt::Notebook>().where("id=?").bind(notebook_id);
    return *nb;
}

void WtDatabase::newNote(model::Note &note)
{
    dbo::Transaction t(session_);
    auto db_note = std::make_unique<wt::Note>(note);
    session_.add(db_note.release());
    t.commit();
}

void WtDatabase::updateNote(const model::Note &note)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Note> db_note =
        session_.find<wt::Note>().where("id=?").bind(note.id());
    auto mod = db_note.modify();
    mod->id(note.id());
    mod->title(note.title());
    mod->content(note.content());
    mod->reminder(note.reminder());
    mod->lastChanged(note.lastChanged());
    t.commit();
}

void WtDatabase::addTag(const int note_id, const int tag_id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Tag> tag = session_.find<wt::Tag>().where("id=?").bind(tag_id);
    dbo::ptr<wt::Note> note =
        session_.find<wt::Note>().where("id=?").bind(note_id);
    tag.modify()->notes.insert(note);
    t.commit();
}

void WtDatabase::removeTag(const int note_id, const int tag_id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Tag> tag = session_.find<wt::Tag>().where("id=?").bind(tag_id);
    dbo::ptr<wt::Note> note =
        session_.find<wt::Note>().where("id=?").bind(note_id);
    tag.modify()->notes.erase(note);
    t.commit();
}

void WtDatabase::deleteNote(const int note_id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Note> note =
        session_.find<wt::Note>().where("id=?").bind(note_id);
    note.remove();
    t.commit();
}

model::Note WtDatabase::loadNote(const int note_id)
{
    dbo::ptr<wt::Note> note =
        session_.find<wt::Note>().where("id=?").bind(note_id);
    return *note;
}

int WtDatabase::newTag(const std::string &title)
{
    dbo::Transaction t(session_);
    auto tag = std::make_unique<wt::Tag>(title);
    dbo::ptr<wt::Tag> tag_ptr = session_.add(tag.release());
    t.commit();
    return tag_ptr->id();
}

std::vector<model::Tag> WtDatabase::listTags()
{
    dbo::collection<dbo::ptr<wt::Tag>> tags = session_.find<wt::Tag>();
    std::vector<model::Tag> result;
    for (const auto &tag_ptr : tags) {
        result.push_back(*tag_ptr);
    }
    return result;
}

void WtDatabase::deleteTag(const int tag_id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Tag> tag = session_.find<wt::Tag>().where("id=?").bind(tag_id);
    tag.remove();
    t.commit();
}

std::vector<model::Note>
WtDatabase::loadNotesFromNotebook(const int notebook_id)
{
    dbo::ptr<wt::Notebook> nb =
        session_.find<wt::Notebook>().where("id=?").bind(notebook_id);

    std::vector<model::Note> result;
    for (const auto &note_ptr : nb->notes) {
        result.push_back(*note_ptr);
    }
    return result;
}

std::vector<model::Note> WtDatabase::loadNotesForTag(const int tag_id)
{
    dbo::ptr<wt::Tag> tag = session_.find<wt::Tag>().where("id=?").bind(tag_id);
    std::vector<model::Note> result;
    for (const auto &note_ptr : tag->notes) {
        result.push_back(*note_ptr);
    }
    return result;
}

std::vector<model::Note> WtDatabase::searchNotes(const std::string &term)
{
    std::cout << "searching in notes for " << term << std::endl;
    return std::vector<model::Note>();
}

} // ns notes
