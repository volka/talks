#include "db/pg_db.h"

#include "db_facade.h"

#include <libpq-fe.h>
#include <iostream>
#include <memory>

namespace notes
{
namespace db
{

// constants
const char *DEFAULT_TIME = "0000-00-00 00:00:00";

// helper for result validation
bool PgDatabase::checkResult(PGresult *res, int expected,
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

int PgDatabase::get_id(PGresult *res)
{
    int fields = PQnfields(res);
    if (fields != 1)
        throw std::runtime_error(
            "RETURNING id query did not return one field as expected");

    int id = -1;
    for (int i = 0; i < PQntuples(res); ++i) {
        id = atoi(PQgetvalue(res, i, 0));
    }
    return id;
}

PgDatabase::PgDatabase(const std::string &connection_info)
    : connection_info_(connection_info)
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

    if (!checkResult(result.get()))
        throw std::runtime_error("dropping pre-existing tables failed");

    result = pg_result_ptr(PQexec(conn, "CREATE TABLE notebooks ("
                                        "id		serial primary key,"
                                        "title	varchar(255)"
                                        ")"),
                           PQclear);
    if (!checkResult(result.get()))
        throw std::runtime_error("creating table notebooks failed");

    result = pg_result_ptr(PQexec(conn, "CREATE TABLE tags ("
                                        "id	 	serial primary key,"
                                        "title	varchar(255)"
                                        ")"),
                           PQclear);

    if (!checkResult(result.get()))
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

    if (!checkResult(result.get()))
        throw std::runtime_error("creating table notes failed");

    result = pg_result_ptr(
        PQexec(conn, "CREATE TABLE tags_nm ("
                     "tag_id		serial references tags(id),"
                     "note_id	serial references notes(id)"
                     ")"),
        PQclear);

    if (!checkResult(result.get()))
        throw std::runtime_error("creating table tags_nm failed");
}

/// Create some sample data for the DB

void PgDatabase::fillDb()
{
    int nb_id = newNotebook("Privat");
    Notebook privat = loadNotebook(nb_id);

    Note folien("Vortrag", "Folien fertigschreiben", privat.id(),
                pt::time_strom_string("2016-03-10 18:59:59"));
    Note code("Beispielcode", "Code schreiben und testen", privat.id(),
              pt::time_strom_string("2016-03-10 18:59:59"));

    int folien_id = newNote(folien);
}

/// Here the real implementation starts!

std::vector<Notebook> PgDatabase::listNotebooks()
{
    return std::vector<Notebook>{};
}

int PgDatabase::newNotebook(const std::string &title)
{
    auto title_str = pg_escaped_ptr(
        PQescapeLiteral(connection_.get(), title.c_str(), title.size()),
        PQfreemem);

    clearStatement();
    stmt_cache_ << "INSERT INTO notebooks(title) VALUES(" << title_str.get()
                << ") RETURNING id";

    auto result = pg_result_ptr(
        PQexec(connection_.get(), stmt_cache_.str().c_str()), PQclear);

    if (!checkResult(result.get(), PGRES_TUPLES_OK))
        throw DatabaseException("inserting notebook " + title + " failed");

    return get_id(result.get());
}

void PgDatabase::renameNotebook(const int notebook_id,
                                const std::string &new_title)
{
    auto title_str = escape(new_title);
    clearStatement();

    stmt_cache_ << "UPDATE notebooks SET (title=" << title_str.get()
                << ") WHERE id=" << notebook_id;
    auto result = executeStatement();

    if (!checkResult(result.get(), PGRES_TUPLES_OK))
        throw DatabaseException("updating notebook title for notebook " +
                                std::to_string(notebook_id) + " failed");
}

void PgDatabase::deleteNotebook(const int notebook_id)
{
    clearStatement();
    stmt_cache_ << "DELETE FROM notebooks WHERE id="
                << std::to_string(notebook_id);
    auto result = pg_result_ptr(
        PQexec(connection_.get(), stmt_cache_.str().c_str()), PQclear);

    if (!checkResult(result.get(), PGRES_TUPLES_OK))
        throw DatabaseException("deleting notebook failed: " +
                                std::to_string(notebook_id));
}

Notebook PgDatabase::loadNotebook(const int notebook_id)
{
    std::cout << "loading notebook " << notebook_id << std::endl;

    clearStatement();
    stmt_cache_ << "SELECT * FROM notebooks WHERE id="
                << std::to_string(notebook_id);
    auto result = executeStatement();

    /// TODO continue here: process result
    return Notebook{};
}

void PgDatabase::newNote(Note &note)
{
    std::cout << "new note " << note.title() << std::endl;

    auto title_str =
        pg_escaped_ptr(PQescapeLiteral(connection_.get(), note.title().c_str(),
                                       note.title().size()),
                       PQfreemem);
    auto content_str = pg_escaped_ptr(PQescapeLiteral(connection_.get(),
                                                      note.content().c_str(),
                                                      note.content().size()),
                                      PQfreemem);

    clearStatement();
    stmt_cache_ << "INSERT INTO notes(title,content,notebook,reminder) VALUES("
                << title_str.get() << ", " << content_str.get() << ", "
                << boost::posix_time::to_iso_string(note.reminder())
                << ") RETURNING id";

    auto result = pg_result_ptr(
        PQexec(connection_.get(), stmt_cache_.str().c_str()), PQclear);

    if (!checkResult(result.get(), PGRES_TUPLES_OK))
        throw DatabaseException("inserting note " + note.title() + " failed");

    return get_id(result.get());
}

void PgDatabase::updateNote(const Note &note)
{
    std::cout << "updateing note " << note.id() << std::endl;
}

void PgDatabase::addTag(const int note_id, const int tag_id)
{
    std::cout << "adding tag " << tag_id << " to note " << note_id << std::endl;
}

void PgDatabase::removeTag(const int note_id, const int tag_id)
{
    std::cout << "remove tag " << tag_id << " from note " << note_id
              << std::endl;
}

void PgDatabase::deleteNote(int id)
{
    std::cout << "deleting note " << id << std::endl;
}

Note PgDatabase::loadNote(int note_id)
{
    std::cout << "loading note " << note_id << std::endl;
    return Note{};
}

int PgDatabase::newTag(const std::string &title)
{
    std::cout << "creating new tag " << title << std::endl;
    return 0;
}

int PgDatabase::findTag(const std::string &title)
{
    std::cout << "searching for tag " << title << std::endl;
    return 0;
}

void PgDatabase::deleteTag(const int tag_id)
{
    std::cout << "deleting tag " << tag_id << std::endl;
}

std::vector<Note> PgDatabase::loadNotesFromNotebook(int notebook_id)
{
    std::cout << "loadNotes: " << notebook_id << std::endl;
    return std::vector<Note>{};
}

std::vector<Note> PgDatabase::loadNotesForTag(int tag_id)
{
    std::cout << "load notes for tag " << tag_id << std::endl;
    return std::vector<Note>{};
}

/// private helper functions

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

} // ns db
} // ns notes
