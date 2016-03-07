#include "db/pg_db.h"

#include "db_facade.h"

#include <libpq-fe.h>
#include <iostream>
#include <memory>
#include <boost/date_time.hpp>

namespace notes
{
namespace db
{

// constants
const char *DEFAULT_TIME = "0000-00-00 00:00:00";

// helper for result validation
bool PgDatabase::checkResultCode(PGresult *res, int expected,
                                 const std::string &msg)
{
    if (res == nullptr || PQresultStatus(res) != expected) {
        if (connection_) {
            std::cout << msg << ": " << PQerrorMessage(connection_.get());
        }
        return false;
    }
    return true;
}

// validate size of PGresult, use expected values < 0 to ignore
bool PgDatabase::checkResultSize(PGresult *res, const int exp_rows,
                                 const int exp_fields)
{
    if (res == nullptr)
        return false;
    if (exp_rows >= 0) {
        if (PQntuples(res) != exp_rows)
            return false;
    }
    if (exp_fields >= 0) {
        if (PQnfields(res) != exp_fields)
            return false;
    }
    return true;
}

// note: no error checking ... use checkResultSize
std::string PgDatabase::getString(PGresult *res, const int row, const int field)
{
    if (PQgetisnull(res, row, field))
        return std::string();
    return std::string(PQgetvalue(res, row, field));
}

// note: no error checking ... use checkResultSize
int PgDatabase::getInt(PGresult *res, const int row, const int field)
{
    if (PQgetisnull(res, row, field))
        return -1;
    return atoi(PQgetvalue(res, row, field));
}

// note: no error checking ... use checkResultSize
pt::ptime PgDatabase::getTimestamp(PGresult *res, const int row,
                                   const int field)
{
    if (PQgetisnull(res, row, field))
        return pt::ptime();
    char *time_str = PQgetvalue(res, row, field);
    try {
        return pt::ptime(pt::time_from_string(time_str));
    } catch (boost::bad_lexical_cast &) {
        return pt::ptime();
    } catch (std::exception &ex) {
        std::cout << "Caught ex " << ex.what() << std::endl;
        return pt::ptime();
    }
}

// get an ID from a query using "RETURNING id"
int PgDatabase::getId(PGresult *res)
{
    int fields = PQnfields(res);
    if (fields != 1)
        throw DatabaseException(
            "RETURNING id query did not return one field as expected");

    int id = -1;
    for (int i = 0; i < PQntuples(res); ++i) {
        id = atoi(PQgetvalue(res, i, 0));
    }
    return id;
}

// RAII wrapper for escaped string
pg_escaped_ptr PgDatabase::escape(const std::string &str)
{
    return pg_escaped_ptr(
        PQescapeLiteral(connection_.get(), str.c_str(), str.size()), PQfreemem);
}

// clear our statement cache
void PgDatabase::clearStatement() { stmt_cache_.str(std::string()); }

// execute a statement using PQexec, wrapping the result in a unique_ptr
pg_result_ptr PgDatabase::executeStatement()
{
    return pg_result_ptr(PQexec(connection_.get(), stmt_cache_.str().c_str()),
                         PQclear);
}

// Constructor / destructor

PgDatabase::PgDatabase(const std::string &connection_info)
    : connection_info_(connection_info), update_note_str_()
{
    connection_ = pg_conn_ptr{PQconnectdb(connection_info_.c_str()), PQfinish};

    if (PQstatus(connection_.get()) != CONNECTION_OK) {
        throw notes::db::DatabaseException(
            "Could not connect to PostgreSQL DB: " +
            std::string(PQerrorMessage(connection_.get())));
    }
}

PgDatabase::~PgDatabase() {}

/// Setup our Notebook database, cleaning all old data !

void PgDatabase::setupDb()
{

    PGconn *conn = connection_.get();

    auto result = pg_result_ptr(
        PQexec(conn, "DROP TABLE IF EXISTS tags_nm, notes, tags, notebooks;"),
        PQclear);

    if (!checkResultCode(result.get()))
        throw std::runtime_error("dropping pre-existing tables failed");

    result = pg_result_ptr(PQexec(conn, "CREATE TABLE notebooks ("
                                        "id		serial primary key,"
                                        "title	varchar(255)"
                                        ")"),
                           PQclear);
    if (!checkResultCode(result.get()))
        throw std::runtime_error("creating table notebooks failed");

    result = pg_result_ptr(PQexec(conn, "CREATE TABLE tags ("
                                        "id	 	serial primary key,"
                                        "title	varchar(255)"
                                        ")"),
                           PQclear);

    if (!checkResultCode(result.get()))
        throw std::runtime_error("creating table tags failed");

    result = pg_result_ptr(
        PQexec(conn, "CREATE TABLE notes ("
                     "id      	serial primary key,"
                     "title   	varchar(255),"
                     "content		text,"
                     "notebook 	int references notebooks(id),"
                     "last_change timestamp DEFAULT CURRENT_TIMESTAMP,"
                     "reminder	timestamp"
                     ")"),
        PQclear);

    if (!checkResultCode(result.get()))
        throw std::runtime_error("creating table notes failed");

    result = pg_result_ptr(
        PQexec(conn, "CREATE TABLE tags_nm ("
                     "tag_id		serial references tags(id),"
                     "note_id	serial references notes(id)"
                     ")"),
        PQclear);

    if (!checkResultCode(result.get()))
        throw std::runtime_error("creating table tags_nm failed");
}

//
// here the NotebookDatabase interface impl. starts

// DEMO: Notebooks are implemented using the "simple" PQexec interface
std::vector<Notebook> PgDatabase::listNotebooks()
{
    clearStatement();
    stmt_cache_ << "SELECT * FROM notebooks";
    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_TUPLES_OK) ||
        !checkResultSize(result.get(), -1, 2))
        throw DatabaseException("listing notebooks failed, invalid result");

    int results = PQntuples(result.get());

    std::vector<Notebook> result_vec;
    result_vec.reserve(results);

    for (int i = 0; i < results; ++i) {
        result_vec.emplace_back(getInt(result.get(), i, 0),
                                getString(result.get(), i, 1));
    }
    return result_vec;
}

int PgDatabase::newNotebook(const std::string &title)
{
    auto title_str = escape(title);

    clearStatement();
    stmt_cache_ << "INSERT INTO notebooks(title) VALUES(" << title_str.get()
                << ") RETURNING id";

    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_TUPLES_OK))
        throw DatabaseException("inserting notebook " + title + " failed");

    return getId(result.get());
}

// Query not returning anything --> PGRES_COMMAND_OK !
void PgDatabase::renameNotebook(const int notebook_id,
                                const std::string &new_title)
{
    auto title_str = escape(new_title);
    clearStatement();

    stmt_cache_ << "UPDATE notebooks SET (title=" << title_str.get()
                << ") WHERE id=" << notebook_id;
    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_COMMAND_OK))
        throw DatabaseException("updating notebook title for notebook " +
                                std::to_string(notebook_id) + " failed");
}

void PgDatabase::deleteNotebook(const int notebook_id)
{
    clearStatement();
    stmt_cache_ << "DELETE FROM notebooks WHERE id="
                << std::to_string(notebook_id);

    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_COMMAND_OK))
        throw DatabaseException("deleting notebook failed: " +
                                std::to_string(notebook_id));
}

Notebook PgDatabase::loadNotebook(const int notebook_id)
{
    clearStatement();
    stmt_cache_ << "SELECT * FROM notebooks WHERE id="
                << std::to_string(notebook_id);

    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_TUPLES_OK) ||
        !checkResultSize(result.get(), 1, 2))
        throw DatabaseException("loading notebook id " +
                                std::to_string(notebook_id) +
                                " failed, invalid result");

    Notebook nb(getInt(result.get(), 0, 0), getString(result.get(), 0, 1));
    return nb;
}

// DEMO : here we use PQexecParams to skip escaping
void PgDatabase::newNote(Note &note)
{
    auto nb_id_str = std::to_string(note.notebook());
    auto date_str = pt::to_iso_string(note.reminder());
    constexpr int num_params = 4;

    const char *paramValues[num_params] = {note.title().c_str(),
                                           note.content().c_str(),
                                           nb_id_str.c_str(), date_str.c_str()};
    const int paramLength[num_params] = {
        static_cast<int>(note.title().size()),
        static_cast<int>(note.content().size()),
        static_cast<int>(nb_id_str.size()), static_cast<int>(date_str.size())};

    clearStatement();
    stmt_cache_ << "INSERT INTO notes(title,content,notebook,reminder) VALUES("
                << "$1, $2, $3, $4"
                << ") RETURNING id";

    auto result = pg_result_ptr(
        PQexecParams(
            connection_.get(), stmt_cache_.str().c_str(),
            num_params,  // number of parameters also defines size of arrays
            nullptr,     // paramTypes, null means infer data type
            paramValues, // the actual parameters
            paramLength, // length of paramValues fields
            nullptr,     // parameter format, nullptr means all text
            0            // use text format for results
            ),
        PQclear);

    if (!checkResultCode(result.get(), PGRES_TUPLES_OK))
        throw DatabaseException("inserting note " + note.title() + " failed");

    // get the generated ID
    note.id(getId(result.get()));
}

// DEMO : use prepared statemenet pg_update_note here!
void PgDatabase::updateNote(const Note &note)
{
    // if not already done, prepare statement
    constexpr int num_params = 5;

    if (update_note_str_.empty()) {
        update_note_str_ = "updateNoteName";
        auto result = pg_result_ptr(
            PQprepare(connection_.get(), update_note_str_.c_str(),
                      "UPDATE notes set (title=$1, content=$2,"
                      "notebook=$3, last_change=CURRENT_TIMESTAMP,"
                      "reminder=$4) WHERE (id=$5)",
                      num_params, nullptr), // note paramTypes here
            PQclear);
        if (!checkResultCode(result.get(), PGRES_COMMAND_OK))
            throw DatabaseException(
                std::string("error preparing UPDATE note statement: ") +
                PQerrorMessage(connection_.get()));
    }

    // simply re-insert all values here
    auto title_str = escape(note.title());
    auto content_str = escape(note.content());
    auto nb_id_str = std::to_string(note.notebook());
    auto date_str = pt::to_iso_string(note.reminder());

    const char *paramValues[num_params] = {title_str.get(), content_str.get(),
                                           nb_id_str.c_str(), date_str.c_str()};
    const int paramLength[num_params] = {
        static_cast<int>(strlen(title_str.get())),
        static_cast<int>(strlen(content_str.get())),
        static_cast<int>(nb_id_str.size()), static_cast<int>(date_str.size())};

    auto result = pg_result_ptr(
        PQexecPrepared(
            connection_.get(), update_note_str_.c_str(), // statement name !
            num_params,  // number of parameters also defines size of arrays
            paramValues, // the actual parameters
            paramLength, // length of paramValues fields
            nullptr,     // parameter format, nullptr means all text
            0            // use text format for results
            ),
        PQclear);

    if (!checkResultCode(result.get(), PGRES_TUPLES_OK))
        throw DatabaseException("inserting note " + note.title() + " failed");
}

void PgDatabase::addTag(const int note_id, const int tag_id)
{
    clearStatement();
    stmt_cache_ << "INSERT INTO tags_nm VALUES(" << std::to_string(tag_id)
                << ", " << std::to_string(note_id) << ")";

    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_COMMAND_OK))
        throw DatabaseException("adding tag " + std::to_string(tag_id) +
                                " to note " + std::to_string(note_id) +
                                " failed");
}

void PgDatabase::removeTag(const int note_id, const int tag_id)
{
    clearStatement();
    if (note_id < 0) {
        stmt_cache_ << "DELETE FROM tags_nm WHERE (tag_id="
                    << std::to_string(tag_id) << ")";
    } else if (tag_id < 0) {
        stmt_cache_ << "DELETE FROM tags_nm WHERE (note_id="
                    << std::to_string(note_id) << ")";
    } else {
        stmt_cache_ << "DELETE FROM tags_nm WHERE (tag_id="
                    << std::to_string(tag_id)
                    << " and note_id=" << std::to_string(note_id) << ")";
    }
    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_COMMAND_OK))
        throw DatabaseException("removing tag " + std::to_string(tag_id) +
                                " for  note " + std::to_string(tag_id) +
                                " failed");
}

void PgDatabase::deleteNote(int note_id)
{
    clearStatement();
    stmt_cache_ << "DELETE FROM notes WHERE (id=" << std::to_string(note_id)
                << ")";

    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_COMMAND_OK))
        throw DatabaseException("deleting note " + std::to_string(note_id) +
                                " failed");
}

Note PgDatabase::loadNote(const int note_id)
{
    clearStatement();
    stmt_cache_ << "SELECT * FROM notes WHERE id=" << std::to_string(note_id);
    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_TUPLES_OK) ||
        !checkResultSize(result.get(), 1, 6)) {
        throw DatabaseException("loading note id " + std::to_string(note_id) +
                                " failed, invalid result");
    }
    return Note(note_id, getString(result.get(), 0, 1), // title
                getString(result.get(), 0, 2),          // content
                getInt(result.get(), 0, 3),             // notebook
                getTimestamp(result.get(), 0, 4),       // last change
                getTimestamp(result.get(), 0, 5)        // reminder
                );
}

int PgDatabase::newTag(const std::string &title)
{
    auto title_str = escape(title);

    clearStatement();
    stmt_cache_ << "INSERT INTO tags(title) VALUES(" << title_str.get()
                << ") RETURNING id";

    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_TUPLES_OK))
        throw DatabaseException("inserting tag " + title + " failed");

    return getId(result.get());
}

std::vector<Tag> db::PgDatabase::listTags()
{
    clearStatement();
    stmt_cache_ << "SELECT * FROM tags order by id asc";
    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_TUPLES_OK) ||
        !checkResultSize(result.get(), -1, 2))
        throw DatabaseException("listing tags failed, invalid result");

    int results = PQntuples(result.get());

    std::vector<Tag> result_vec;
    result_vec.reserve(results);

    for (int i = 0; i < results; ++i) {
        result_vec.emplace_back(getInt(result.get(), i, 0),
                                getString(result.get(), i, 1));
    }
    return result_vec;
}

void PgDatabase::deleteTag(const int tag_id)
{
    clearStatement();
    stmt_cache_ << "DELETE FROM tags WHERE (id=" << std::to_string(tag_id)
                << ")";

    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_COMMAND_OK))
        throw DatabaseException("deleting tag " + std::to_string(tag_id) +
                                " failed (" + stmt_cache_.str() + ")");
}

std::vector<Note> PgDatabase::loadNotesFromNotebook(const int notebook_id)
{
    clearStatement();
    stmt_cache_ << "SELECT * FROM notes where (notebook="
                << std::to_string(notebook_id) << ") order by id asc";
    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_TUPLES_OK) ||
        !checkResultSize(result.get(), -1, 6))
        throw DatabaseException("listing notes failed, invalid result (" +
                                stmt_cache_.str() + ")");

    int results = PQntuples(result.get());

    std::vector<Note> result_vec;
    result_vec.reserve(results);

    for (int i = 0; i < results; ++i) {
        result_vec.emplace_back(
            getInt(result.get(), i, 0), getString(result.get(), i, 1),
            getString(result.get(), i, 2), getInt(result.get(), i, 3),
            getTimestamp(result.get(), i, 4), getTimestamp(result.get(), i, 5));
    }
    return result_vec;
}

std::vector<Note> PgDatabase::loadNotesForTag(const int tag_id)
{
    clearStatement();
    stmt_cache_
        << "SELECT notes.id, notes.title, notes.content, notes.notebook, "
           "notes.last_change, notes.reminder FROM notes join tags_nm ON "
           "(notes.id=tags_nm.note_id)"
           " WHERE (tag_id = " << std::to_string(tag_id)
        << ") ORDER BY notes.id asc";
    auto result = executeStatement();

    if (!checkResultCode(result.get(), PGRES_TUPLES_OK) ||
        !checkResultSize(result.get(), -1, 6))
        throw DatabaseException("listing notes failed, invalid result");

    int results = PQntuples(result.get());

    std::vector<Note> result_vec;
    result_vec.reserve(results);

    for (int i = 0; i < results; ++i) {
        result_vec.emplace_back(
            getInt(result.get(), i, 0), getString(result.get(), i, 1),
            getString(result.get(), i, 2), getInt(result.get(), i, 3),
            getTimestamp(result.get(), i, 4), getTimestamp(result.get(), i, 5));
    }
    return result_vec;
}

// DEMO : search using like and joins
std::vector<Note> PgDatabase::searchNotes(const std::string &term)
{
    clearStatement();
    stmt_cache_
        << "SELECT notes.id, notes.title, notes.content, notes.notebook, "
           "notes.last_change, notes.reminder FROM notes left join tags_nm ON "
           "(notes.id=tags_nm.note_id) left join tags ON "
           "(tags_nm.tag_id=tags.id)"
           "WHERE (notes.title ilike '%" << term
        << "%' or notes.content ilike '%" << term << "%' or tags.title ilike '%"
        << term << "%') order by notes.id asc";

    std::cout << "Query: " << stmt_cache_.str() << std::endl;
    auto result = executeStatement();
    if (!checkResultCode(result.get(), PGRES_TUPLES_OK) ||
        !checkResultSize(result.get(), -1, 6))
        throw DatabaseException("listing notes failed, invalid result");

    int results = PQntuples(result.get());

    std::vector<Note> result_vec;
    result_vec.reserve(results);

    for (int i = 0; i < results; ++i) {
        result_vec.emplace_back(
            getInt(result.get(), i, 0), getString(result.get(), i, 1),
            getString(result.get(), i, 2), getInt(result.get(), i, 3),
            getTimestamp(result.get(), i, 4), getTimestamp(result.get(), i, 5));
    }
    return result_vec;
}

} // ns db
} // ns notes
