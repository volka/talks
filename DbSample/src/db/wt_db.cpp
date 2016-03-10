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
        auto sqlite3 = std::make_unique<dbo::backend::Sqlite3>(config_.dbname);
        sqlite3->setDateTimeStorage(
            dbo::SqlDateTimeType::SqlDateTime,
            dbo::backend::Sqlite3::DateTimeStorage::ISO8601AsText);
        conn_.reset(sqlite3.release());
        session_.setConnection(*conn_);
    } else if (config_.driver == "PG") {
        conn_.reset(new dbo::backend::Postgres(config_.conn_str));
        session_.setConnection(*conn_);
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
    dbo::Transaction t(session_);
    // try {
    //    session_.dropTables();
    //} catch (dbo::Exception& ex) {
    //    std::cout << "Ignoring exeption during dropTables: "
    //              << ex.what() << std::endl;
    //}
    session_.createTables();
    t.commit();
}

// listing per find - WARNING: result is sliced !
std::vector<model::Notebook> WtDatabase::listNotebooks()
{
    dbo::Transaction t(session_);
    std::vector<model::Notebook> result;

    dbo::collection<dbo::ptr<wt::Notebook>> nb_list =
        session_.find<wt::Notebook>();
    for (const auto &nb : nb_list) {
        result.emplace_back(nb.id(), nb->title());
    }
    t.commit();
    return result;
}

// we can use unique_ptrs for resource management before adding values to
// session_
bigint_t WtDatabase::newNotebook(const std::string &title)
{
    dbo::Transaction t(session_);
    auto nb = std::make_unique<wt::Notebook>(title);
    dbo::ptr<wt::Notebook> nb_ptr = session_.add(nb.release());
    t.commit();
    // WARNING : I am using Wt::Dbo IDs here instead of the id() defined on my
    // objects
    std::cout << "created new notebook with id " << nb_ptr.id() << std::endl;
    return nb_ptr.id();
}

// find() / modify() example
void WtDatabase::renameNotebook(const bigint_t notebook_id,
                                const std::string &new_title)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Notebook> nb =
        session_.find<wt::Notebook>().where("id=?").bind(notebook_id);
    nb.modify()->title(new_title);
    t.commit();
}

void WtDatabase::deleteNotebook(const bigint_t id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Notebook> nb =
        session_.find<wt::Notebook>().where("id=?").bind(id);
    nb.remove();
    t.commit();
}

model::Notebook WtDatabase::loadNotebook(const bigint_t notebook_id)
{
    model::Notebook model_nb;
    dbo::Transaction t(session_);
    dbo::ptr<wt::Notebook> nb =
        session_.find<wt::Notebook>().where("id=?").bind(notebook_id);
    t.commit();
    model_nb = *nb;
    model_nb.id(nb.id());
    return model_nb;
}

void WtDatabase::newNote(model::Note &note)
{
    dbo::Transaction t(session_);
    auto db_note = std::make_unique<wt::Note>(note);
    // we need to add the notebook manually
    dbo::ptr<wt::Notebook> nb =
        session_.find<wt::Notebook>().where("id=?").bind(note.notebook());
    db_note->notebook = nb;
    dbo::ptr<wt::Note> new_note = session_.add(db_note.release());
    t.commit();
    note.id(new_note.id());
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

void WtDatabase::addTag(const bigint_t note_id, const bigint_t tag_id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Tag> tag = session_.find<wt::Tag>().where("id=?").bind(tag_id);
    dbo::ptr<wt::Note> note =
        session_.find<wt::Note>().where("id=?").bind(note_id);
    tag.modify()->notes.insert(note);
    t.commit();
}

void WtDatabase::removeTag(const bigint_t note_id, const bigint_t tag_id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Tag> tag = session_.find<wt::Tag>().where("id=?").bind(tag_id);
    dbo::ptr<wt::Note> note =
        session_.find<wt::Note>().where("id=?").bind(note_id);
    tag.modify()->notes.erase(note);
    t.commit();
}

void WtDatabase::deleteNote(const bigint_t note_id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Note> note =
        session_.find<wt::Note>().where("id=?").bind(note_id);
    note.remove();
    t.commit();
}

model::Note WtDatabase::loadNote(const bigint_t note_id)
{
    model::Note model_note;
    dbo::Transaction t(session_);
    dbo::ptr<wt::Note> note =
        session_.find<wt::Note>().where("id=?").bind(note_id);
    t.commit();
    model_note = *note;
    model_note.id(note.id());
    model_note.notebook(note->notebook.id());
    return model_note;
}

bigint_t WtDatabase::newTag(const std::string &title)
{
    dbo::Transaction t(session_);
    auto tag = std::make_unique<wt::Tag>(title);
    dbo::ptr<wt::Tag> tag_ptr = session_.add(tag.release());
    t.commit();
    return tag_ptr.id();
}

std::vector<model::Tag> WtDatabase::listTags()
{
    dbo::Transaction t(session_);
    dbo::collection<dbo::ptr<wt::Tag>> tags = session_.find<wt::Tag>();
    std::vector<model::Tag> result;
    for (const auto &tag_ptr : tags) {
        result.emplace_back(tag_ptr.id(), tag_ptr->title());
    }
    t.commit();
    return result;
}

void WtDatabase::deleteTag(const bigint_t tag_id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Tag> tag = session_.find<wt::Tag>().where("id=?").bind(tag_id);
    tag.remove();
    t.commit();
}

std::vector<model::Note>
WtDatabase::loadNotesFromNotebook(const bigint_t notebook_id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Notebook> nb =
        session_.find<wt::Notebook>().where("id=?").bind(notebook_id);

    std::cout << "found notebook notes: " << nb.modify()->notes.size()
              << std::endl;

    std::vector<model::Note> result;
    model::Note tmp;
    for (dbo::collection<dbo::ptr<wt::Note>>::const_iterator it =
             nb->notes.begin();
         it != nb->notes.end(); ++it) {
        std::cout << "found note " << (*it).id() << " - " << (*it)->title()
                  << std::endl;
        tmp = **it;
        tmp.id((*it).id());
        result.push_back(tmp);
    }
    t.commit();
    return result;
}

std::vector<model::Note> WtDatabase::loadNotesForTag(const bigint_t tag_id)
{
    dbo::Transaction t(session_);
    dbo::ptr<wt::Tag> tag = session_.find<wt::Tag>().where("id=?").bind(tag_id);
    std::vector<model::Note> result;
    model::Note tmp;
    for (const auto &note_ptr : tag->notes) {
        tmp = *note_ptr;
        tmp.id(note_ptr.id());
        result.push_back(tmp);
    }
    t.commit();
    return result;
}

std::vector<model::Note> WtDatabase::searchNotes(const std::string &term)
{
    dbo::Transaction t(session_);
    std::cout << "searching in notes for " << term << std::endl;
    t.commit();
    // TODO : not implemented
    std::vector<model::Note> result;
    if (term == "C++") {
        result.emplace_back(0,"test", "test", 0, pt::ptime(), pt::ptime());
        result.emplace_back(1,"test", "test", 0, pt::ptime(), pt::ptime());
    } else {
        result.emplace_back(2,"Essen", "Eier", 1, pt::ptime(), pt::ptime());
    }
    return result;
}

} // ns notes
