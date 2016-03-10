#include "db/sqlpp_db.h"
#include "db/sqlpp_model.h"
#include <iostream>
#include <chrono>

namespace notes
{
namespace db
{
using namespace sqlpp;
using namespace std::chrono;

SqlppDatabase::SqlppDatabase(const std::string &connection_info)
    : connection_info_(connection_info)
{
    sqlite3::connection_config config;
    config.path_to_database = connection_info;
    config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    config.debug = true;

    connection_.reset(new sqlite3::connection(config));
}

SqlppDatabase::~SqlppDatabase() {}

void SqlppDatabase::setupDb()
{
    conn().execute("DROP TABLE IF EXISTS tags_nm;");
    conn().execute("DROP TABLE IF EXISTS notes;");
    conn().execute("DROP TABLE IF EXISTS tags;");
    conn().execute("DROP TABLE IF EXISTS notebooks;");
    conn().execute("CREATE TABLE notebooks ("
                   "id		integer primary key,"
                   "title	varchar(255)"
                   ")");
    conn().execute("CREATE TABLE tags ("
                   "id	 	integer primary key,"
                   "title	varchar(255)"
                   ")");
    conn().execute(
        "CREATE TABLE notes ("
        "id      	integer primary key,"
        "title   	varchar(255),"
        "content	text,"
        "notebook 	int references notebooks(id) ON DELETE CASCADE,"
        "last_change timestamp DEFAULT (datetime('now','localtime')),"
        "reminder	timestamp"
        ")");
    conn().execute("CREATE TABLE tags_nm ("
                   "tag_id	integer references tags(id)"
                   " ON DELETE CASCADE,"
                   "note_id	integer references notes(id)"
                   " ON DELETE CASCADE"
                   ")");
}

std::vector<Notebook> SqlppDatabase::listNotebooks()
{
    auto stmt = select(all_of(notebooks_)).from(notebooks_).where(true);

    std::vector<Notebook> result;
    for (const auto &row : conn()(stmt)) {
        result.emplace_back(row.id, row.title);
    }
    return result;
}

bigint_t SqlppDatabase::newNotebook(const std::string &title)
{
    auto stmt = insert_into(notebooks_).set(notebooks_.title = title);
    // sqlite connector returns last inserted ID
    return conn()(stmt);
}

void SqlppDatabase::renameNotebook(const bigint_t notebook_id,
                                   const std::string &new_title)
{
    auto stmt = update(notebooks_).set(notebooks_.title = new_title).where(
        notebooks_.id == notebook_id);
    conn()(stmt);
}

void SqlppDatabase::deleteNotebook(const bigint_t id)
{
    auto stmt = remove_from(notebooks_).where(notebooks_.id == id);
    conn()(stmt);
}

Notebook SqlppDatabase::loadNotebook(const bigint_t notebook_id)
{
    auto stmt = select(all_of(notebooks_)).from(notebooks_).where(
        notebooks_.id == notebook_id);
    for (auto &row : conn()(stmt)) {
        return Notebook(row.id, row.title);
    }
    return Notebook();
}

void SqlppDatabase::newNote(Note &note)
{
    using namespace std::chrono;
    auto reminder = system_clock::from_time_t(pt::to_time_t(note.reminder()));

    auto stmt = insert_into(notes_).set(
        notes_.title = note.title(), notes_.content = note.content(),
        notes_.notebook = note.notebook(), notes_.reminder = reminder);

    note.id(conn()(stmt));
}

void SqlppDatabase::updateNote(const Note &note)
{
    using namespace std::chrono;
    auto reminder = system_clock::from_time_t(pt::to_time_t(note.reminder()));
    auto stmt =
        update(notes_)
            .set(notes_.title = note.title(), notes_.content = note.content(),
                 notes_.notebook = note.notebook(), notes_.reminder = reminder)
            .where(notes_.id == note.id());
    conn()(stmt);
}

void SqlppDatabase::addTag(const bigint_t note_id, const bigint_t tag_id)
{
    auto stmt = insert_into(tags_nm_)
                    .set(tags_nm_.tag_id = tag_id, tags_nm_.note_id = note_id);
    conn()(stmt);
}

void SqlppDatabase::removeTag(const bigint_t note_id, const bigint_t tag_id)
{
    auto stmt = remove_from(tags_nm_).where(tags_nm_.tag_id == tag_id and
                                            tags_nm_.note_id == note_id);
    conn()(stmt);
}

void SqlppDatabase::deleteNote(const bigint_t id)
{
    auto stmt = remove_from(notes_).where(notes_.id == id);
    conn()(stmt);
}

Note SqlppDatabase::loadNote(const bigint_t note_id)
{
    auto stmt = select(all_of(notes_)).from(notes_).where(notes_.id == note_id);
    for (auto &row : conn()(stmt)) {
        // only 1 iteration
        return Note(
            row.id, row.title, row.content, row.notebook,
            pt::from_time_t(system_clock::to_time_t(row.last_change.value())),
            pt::from_time_t(system_clock::to_time_t(row.reminder.value())));
    }
    return Note();
}

bigint_t SqlppDatabase::newTag(const std::string &title)
{
    auto stmt = insert_into(tags_).set(tags_.title = title);
    return conn()(stmt);
}

std::vector<Tag> db::SqlppDatabase::listTags()
{
    auto stmt = select(all_of(tags_)).from(tags_).where(true);

    std::vector<model::Tag> result;
    for (const auto &row : conn()(stmt)) {
        result.emplace_back(row.id, row.title);
    }
    return result;
}

void SqlppDatabase::deleteTag(const bigint_t tag_id)
{
    auto stmt = remove_from(tags_).where(tags_.id == tag_id);
    conn()(stmt);
}

std::vector<Note>
SqlppDatabase::loadNotesFromNotebook(const bigint_t notebook_id)
{
    auto stmt = select(all_of(notes_)).from(notes_).where(notes_.notebook ==
                                                          notebook_id);

    std::vector<Note> result;
    for (auto &row : conn()(stmt)) {
        result.push_back(Note(
            row.id, row.title, row.content, row.notebook,
            pt::from_time_t(system_clock::to_time_t(row.last_change.value())),
            pt::from_time_t(system_clock::to_time_t(row.reminder.value()))));
    }
    return result;
}

std::vector<Note> SqlppDatabase::loadNotesForTag(const bigint_t tag_id)
{
    auto stmt =
        select(all_of(notes_))
            .from(notes_.join(tags_nm_).on(tags_nm_.note_id == notes_.id))
            .where(tags_nm_.tag_id == tag_id);

    std::vector<Note> result;
    for (auto &row : conn()(stmt)) {
        result.push_back(Note(
            row.id, row.title, row.content, row.notebook,
            pt::from_time_t(system_clock::to_time_t(row.last_change.value())),
            pt::from_time_t(system_clock::to_time_t(row.reminder.value()))));
    }
    return result;
}

std::vector<Note> SqlppDatabase::searchNotes(const std::string &term)
{
    std::string search_term = "%" + term + "%";
    auto stmt = select(all_of(notes_))
                    .from(notes_.left_outer_join(tags_nm_)
                              .on(notes_.id == tags_nm_.note_id)
                              .left_outer_join(tags_)
                              .on(tags_nm_.tag_id == tags_.id))
                    .where(notes_.title.like(search_term) or
                           notes_.content.like(search_term) or
                           tags_.title.like(search_term));

    std::vector<Note> result;
    for (auto &row : conn()(stmt)) {
        result.push_back(Note(
            row.id, row.title, row.content, row.notebook,
            pt::from_time_t(system_clock::to_time_t(row.last_change.value())),
            pt::from_time_t(system_clock::to_time_t(row.reminder.value()))));
    }
    return result;
}

sqlite3::connection &SqlppDatabase::conn()
{
    if (!connection_)
        throw DatabaseException("sqlpp11::sqlite3 connection invalid");
    return *connection_;
}

} // ns db
} // ns notes
