#include "db/sqlite_db.h"
#include <iostream>

namespace notes
{
namespace db
{

Sqlite3Database::Sqlite3Database(const std::string &connection_info)
    : connection_info_(connection_info), connection_{nullptr}
{
    sqlite3 *conn;
    int result = sqlite3_open(connection_info.c_str(), &conn);
    if (result == SQLITE_OK) {
        // enable foreign key checking ...
        sqlite3_exec(conn, "PRAGMA foreign_keys = ON;", nullptr, nullptr,
                     nullptr);
        connection_ = sqlite_conn(conn);
    } else {
        throw DatabaseException("Error opening SQLite database " +
                                connection_info + " (error: " +
                                std::to_string(result) + ")");
    }
}

Sqlite3Database::~Sqlite3Database() {}

int Sqlite3Database::getLastInsertId()
{
    return static_cast<int>(sqlite3_last_insert_rowid(connection_.ptr()));
}

int Sqlite3Database::getInt(const sqlite_stmt &stmt, const int column)
{
    return sqlite3_column_int(stmt.ptr(), column);
}

std::string Sqlite3Database::getString(const sqlite_stmt &stmt,
                                       const int column)
{
    const unsigned char *text_result = sqlite3_column_text(stmt.ptr(), column);
    if (text_result == nullptr)
        return std::string();

    return std::string(reinterpret_cast<const char *>(text_result));
}

pt::ptime Sqlite3Database::getTimestamp(const sqlite_stmt &stmt,
                                        const int column)
{
    const unsigned char *text_result = sqlite3_column_text(stmt.ptr(), column);
    if (text_result == nullptr)
        return pt::ptime();

    try {
        return pt::from_iso_string(reinterpret_cast<const char *>(text_result));
    } catch (boost::bad_lexical_cast &) {
        return pt::ptime();
    }
}

int Sqlite3Database::bindString(sqlite_stmt &stmt, const int pos,
                                const std::string &str)
{
    // note 5th parameter: Destructor for char* OR SQLITE_STATIC for no
    // destruction
    //                     SQLITE_TRANSIENT to instruct Sqlite to make a copy
    return sqlite3_bind_text(stmt.ptr(), pos, str.c_str(),
                             static_cast<int>(str.size()), SQLITE_STATIC);
}

int Sqlite3Database::bindInt(sqlite_stmt &stmt, const int pos, const int val)
{
    return sqlite3_bind_int(stmt.ptr(), pos, val);
}

sqlite_stmt Sqlite3Database::prepareStatement(const std::string &stmt)
{
    sqlite3_stmt *stmt_ptr;
    // connection, statement string, length, OUT stmt pointer, ignored "unused
    // part of stmt"
    int res =
        sqlite3_prepare_v2(connection_.ptr(), stmt.c_str(),
                           static_cast<int>(stmt.size()), &stmt_ptr, nullptr);
    checkResult(res, SQLITE_OK, "Error preparing statement " + stmt, true);
    return sqlite_stmt(stmt_ptr);
}

bool Sqlite3Database::isError(const int res) const
{
    switch (res) {
    case SQLITE_OK:
    case SQLITE_DONE:
    case SQLITE_ROW:
        return false;
    default:
        return true;
    }
    return false;
}

int Sqlite3Database::executeStep(sqlite_stmt &stmt)
{
    int res = sqlite3_step(stmt.ptr());
    if (isError(res)) {
        throw DatabaseException("Error binding result");
    }
    return res;
}

bool Sqlite3Database::checkResult(int result, int expected,
                                  const std::string &msg, bool do_throw) const
{
    if (result != expected) {
        auto errmsg = msg + ": " + std::to_string(result) + " " +
                      std::string(sqlite3_errmsg(connection_.ptr()));
        if (do_throw) {
            throw DatabaseException(errmsg);
        } else {
            std::cout << errmsg << std::endl;
        }
        return false;
    }
    return true;
}

// create database structure
void Sqlite3Database::setupDb()
{
    auto result = prepareStatement("DROP TABLE IF EXISTS tags_nm;");
    if (isError(executeStep(result)))
        throw DatabaseException("dropping table notebooks failed");

    result = prepareStatement("DROP TABLE IF EXISTS notes;");
    if (isError(executeStep(result)))
        throw DatabaseException("dropping table notebooks failed");

    result = prepareStatement("DROP TABLE IF EXISTS tags;");
    if (isError(executeStep(result)))
        throw DatabaseException("dropping table tags failed");

    result = prepareStatement("DROP TABLE IF EXISTS notebooks;");
    if (isError(executeStep(result)))
        throw DatabaseException("dropping table tags failed");

    result = prepareStatement("CREATE TABLE notebooks ("
                              "id		integer primary key,"
                              "title	varchar(255)"
                              ")");

    if (isError(executeStep(result)))
        throw DatabaseException("creating table notebooks failed");

    result = prepareStatement("CREATE TABLE tags ("
                              "id	 	integer primary key,"
                              "title	varchar(255)"
                              ")");

    if (isError(executeStep(result)))
        throw DatabaseException("creating table tags failed");

    result = prepareStatement(
        "CREATE TABLE notes ("
        "id      	integer primary key,"
        "title   	varchar(255),"
        "content		text,"
        "notebook 	int references notebooks(id),"
        "last_change timestamp DEFAULT (datetime('now','localtime')),"
        "reminder	timestamp"
        ")");

    if (isError(executeStep(result)))
        throw DatabaseException("creating table notes failed");

    result = prepareStatement("CREATE TABLE tags_nm ("
                              "tag_id		integer references tags(id),"
                              "note_id	integer references notes(id)"
                              ")");

    if (isError(executeStep(result)))
        throw DatabaseException("creating table tags_nm failed");
}

// implementation of NotebookDatabase interface
std::vector<Notebook> Sqlite3Database::listNotebooks()
{
    auto result = prepareStatement("SELECT * FROM notebooks");
    int status = executeStep(result);
    if (isError(status))
        throw DatabaseException("listing notebooks failed, invalid result");

    std::vector<Notebook> result_vec;
    while (status != SQLITE_DONE) {
        result_vec.emplace_back(getInt(result, 0), getString(result, 1));
        status = executeStep(result);
    }
    return result_vec;
}

int Sqlite3Database::newNotebook(const std::string &title)
{
    clearStatement();
    stmt_cache_ << "INSERT INTO notebooks(title) VALUES('" << title << "')";
    auto result = prepareStatement(stmt_cache_.str());
    if (isError(executeStep(result)))
        throw DatabaseException("inserting notebook " + title + " failed");

    return getLastInsertId();
}

void Sqlite3Database::renameNotebook(const int notebook_id,
                                     const std::string &new_title)
{
    clearStatement();
    stmt_cache_ << "UPDATE notebooks SET (title='" << new_title
                << "') WHERE id=" << notebook_id;
    auto result = prepareStatement(stmt_cache_.str());

    if (isError(executeStep(result)))
        throw DatabaseException("updating notebook title for notebook " +
                                new_title);
}

void Sqlite3Database::deleteNotebook(const int notebook_id)
{
    clearStatement();
    stmt_cache_ << "DELETE FROM notebooks WHERE id="
                << std::to_string(notebook_id);

    auto result = prepareStatement(stmt_cache_.str());

    if (isError(executeStep(result)))
        throw DatabaseException("deleting notebook failed: " +
                                std::to_string(notebook_id));
}

Notebook Sqlite3Database::loadNotebook(const int notebook_id)
{
    clearStatement();
    stmt_cache_ << "SELECT * FROM notebooks WHERE id="
                << std::to_string(notebook_id);

    auto result = prepareStatement(stmt_cache_.str());

    if (isError(executeStep(result)))
        throw DatabaseException("loading notebook id " +
                                std::to_string(notebook_id) +
                                " failed, invalid result");

    Notebook nb(getInt(result, 0), getString(result, 1));

    return nb;
}

// DEMO : sqlite3 example with prepared statement parameters
void Sqlite3Database::newNote(Note &note)
{
    auto date_str = pt::to_iso_string(note.reminder());

    clearStatement();
    stmt_cache_ << "INSERT INTO notes(title,content,notebook,reminder) VALUES("
                << "?1, ?2, ?3, ?4"
                << ")";

    auto result = prepareStatement(stmt_cache_.str());

    bindString(result, 1, note.title());
    bindString(result, 2, note.content());
    bindInt(result, 3, note.notebook());
    bindString(result, 4, date_str);
    if (isError(executeStep(result)))
        throw DatabaseException("inserting note " + note.title() + " failed");

    // get the generated ID
    note.id(static_cast<int>(getLastInsertId()));
}

void Sqlite3Database::updateNote(const Note &note)
{
    auto date_str = pt::to_iso_string(note.reminder());

    clearStatement();
    stmt_cache_ << "UPDATE notes SET(title=?1,content=?2,"
                << "notebook=?3,last_change=datetime('now','localtime'),"
                << "reminder=?4)";

    auto result = prepareStatement(stmt_cache_.str());

    bindString(result, 1, note.title());
    bindString(result, 2, note.content());
    bindInt(result, 3, note.notebook());
    bindString(result, 4, date_str);
    if (isError(executeStep(result)))
        throw DatabaseException("inserting note " + note.title() + " failed");
}

void Sqlite3Database::addTag(const int note_id, const int tag_id)
{

    clearStatement();
    stmt_cache_ << "INSERT INTO tags_nm VALUES(" << std::to_string(tag_id)
                << ", " << std::to_string(note_id) << ")";
    auto result = prepareStatement(stmt_cache_.str());
    if (isError(executeStep(result)))
        throw DatabaseException("adding tag " + std::to_string(tag_id) +
                                " to " + std::to_string(note_id) + " failed");
}

void Sqlite3Database::removeTag(const int note_id, const int tag_id)
{

    clearStatement();
    if (note_id < 0) {
        stmt_cache_ << "DELETE FROM tags_nm WHERE (tag_id="
                    << std::to_string(tag_id) << ")";
    } else if (tag_id < 0) {
        stmt_cache_ << "DELETE FROM tags_nm WHERE (note_id="
                    << std::to_string(note_id) << ")";
    } else {
        stmt_cache_ << "DELETE FROM tags_nm WHERE (note_id="
                    << std::to_string(note_id)
                    << " and tag_id=" << std::to_string(tag_id) << ")";
    }
    auto result = prepareStatement(stmt_cache_.str());
    if (isError(executeStep(result)))
        throw DatabaseException("removing tag " + std::to_string(tag_id) +
                                " failed");
}

void Sqlite3Database::deleteNote(const int note_id)
{
    clearStatement();
    stmt_cache_ << "DELETE FROM notes WHERE id=" << std::to_string(note_id);
    auto result = prepareStatement(stmt_cache_.str());
    if (isError(executeStep(result)))
        throw DatabaseException("deleting note " + std::to_string(note_id) +
                                " failed");
}

int Sqlite3Database::newTag(const std::string &title)
{
    clearStatement();
    stmt_cache_ << "INSERT INTO tags(title) VALUES('" << title << "')";
    auto result = prepareStatement(stmt_cache_.str());
    if (isError(executeStep(result)))
        throw DatabaseException("inserting tag " + title + " failed");

    return getLastInsertId();
}

std::vector<Tag> db::Sqlite3Database::listTags()
{
    clearStatement();
    stmt_cache_ << "SELECT * FROM tags";
    auto result = prepareStatement(stmt_cache_.str());
    int state = SQLITE_OK;

    std::vector<Tag> result_vec;
    do {
        state = executeStep(result);
        if (isError(state))
            throw DatabaseException("listing tags failed, invalid result");
        if (state == SQLITE_DONE)
            break;

        result_vec.emplace_back(getInt(result, 0), getString(result, 1));
    } while (!isError(state) && state != SQLITE_DONE);
    return result_vec;
}

void Sqlite3Database::deleteTag(const int tag_id)
{
    clearStatement();
    stmt_cache_ << "DELETE FROM tags WHERE id=" << std::to_string(tag_id);
    auto result = prepareStatement(stmt_cache_.str());
    if (isError(executeStep(result)))
        throw DatabaseException("deleting tag " + std::to_string(tag_id) +
                                " failed");
}

Note Sqlite3Database::loadNote(const int note_id)
{
    clearStatement();
    stmt_cache_ << "SELECT * FROM notes WHERE id=" << std::to_string(note_id);
    auto result = prepareStatement(stmt_cache_.str());

    if (isError(executeStep(result))) {
        throw DatabaseException("loading note id " + std::to_string(note_id) +
                                " failed, invalid result");
    }
    return Note(note_id, getString(result, 1), // title
                getString(result, 2),          // content
                getInt(result, 3),             // notebook
                getTimestamp(result, 4),       // last change
                getTimestamp(result, 5)        // reminder
                );
}

std::vector<Note> Sqlite3Database::loadNotesFromNotebook(int notebook_id)
{
    clearStatement();
    stmt_cache_ << "SELECT * FROM notes where (notebook="
                << std::to_string(notebook_id) << ")";
    auto result = prepareStatement(stmt_cache_.str());
    int state = SQLITE_OK;

    std::vector<Note> result_vec;
    do {
        state = executeStep(result);
        if (isError(state))
            throw DatabaseException("listing notes failed, invalid result");
        if (state == SQLITE_DONE)
            break;

        result_vec.emplace_back(getInt(result, 0), getString(result, 1),
                                getString(result, 2), getInt(result, 3),
                                getTimestamp(result, 4),
                                getTimestamp(result, 5));
    } while (!isError(state) && state != SQLITE_DONE);
    return result_vec;
}

std::vector<Note> Sqlite3Database::loadNotesForTag(int tag_id)
{
    clearStatement();
    stmt_cache_
        << "SELECT notes.id, notes.title, notes.content, notes.last_change, "
        << "notes.reminder FROM notes join tags_nm ON "
           "(notes.id=tags_nm.note_id)"
        << " WHERE (tag_id = " << std::to_string(tag_id) << ")";
    auto result = prepareStatement(stmt_cache_.str());
    int state = SQLITE_OK;

    std::vector<Note> result_vec;
    do {
        state = executeStep(result);
        if (isError(state))
            throw DatabaseException("listing notes failed, invalid result");
        if (state == SQLITE_DONE)
            break;

        result_vec.emplace_back(getInt(result, 0), getString(result, 1),
                                getString(result, 2), getInt(result, 3),
                                getTimestamp(result, 4),
                                getTimestamp(result, 5));
    } while (state != SQLITE_DONE);
    return result_vec;
}

std::vector<Note> Sqlite3Database::searchNotes(const std::string &term)
{
    clearStatement();
    stmt_cache_
        << "SELECT notes.id, notes.title, notes.content, notes.last_change, "
        << "notes.reminder FROM notes left join tags_nm ON "
           "(notes.id=tags_nm.note_id)"
        << " left join tags ON (tags_nm.tag_id=tags.id) WHERE ("
        << " notes.title ilike '%" << term << "%' or notes.content ilike '%"
        << term << "%' or tags.title ilike '%" << term << "%')";
    auto result = prepareStatement(stmt_cache_.str());
    int state = SQLITE_OK;

    std::vector<Note> result_vec;
    do {
        state = executeStep(result);
        if (isError(state))
            throw DatabaseException("Searching for notes failed with ec=" +
                                    std::to_string(state));
        if (state == SQLITE_DONE)
            break;

        result_vec.emplace_back(getInt(result, 0), getString(result, 1),
                                getString(result, 2), getInt(result, 3),
                                getTimestamp(result, 4),
                                getTimestamp(result, 5));
    } while (state != SQLITE_DONE);
    return result_vec;
}

} // ns db
} // ns notes
