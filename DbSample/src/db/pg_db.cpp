#include "pg/pg_db.h"

#include "db_facade.h"

#include <libpq-fe.h>
#include <iostream>
#include <memory>

namespace notes {
namespace db {

// constants
const char *DEFAULT_TIME = "0000-00-00 00:00:00";

// helper for result validation
bool checkResult(PGconn *conn, PGresult *res, int expected = PGRES_COMMAND_OK,
                 const std::string &msg = "Command failed") {
    if (res == nullptr || PQresultStatus(res) != expected) {
        std::cout << msg << ": " << PQerrorMessage(conn);
        return false;
    }
    return true;
}

int get_id(PGresult *res) {
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
    : connection_info_(connection_info) {
    connection_ = pg_conn_ptr{PQconnectdb(connection_info_.c_str()), PQfinish};

    if (PQstatus(connection_.get()) != CONNECTION_OK) {
        throw notes::db::DatabaseException(
            "Could not connect to PostgreSQL DB: " +
            std::string(PQerrorMessage(connection_.get())));
    }
}

PgDatabase::~PgDatabase() {}

void PgDatabase::setupDb() {}

void PgDatabase::fillDb() {}

int PgDatabase::newNotebook(const std::string &title) {
    std::cout << "new notebook " << title << std::endl;
    return 0;
}

void PgDatabase::renameNotebook(const int notebook_id,
                                const std::string &new_title) {
    std::cout << "rename notebook " << notebook_id << " to " << new_title
              << std::endl;
}

void PgDatabase::deleteNotebook(const int id) {

    std::cout << "deleting notebook " << id << std::endl;
}

std::unique_ptr<Notebook> PgDatabase::loadNotebook(const int notebook_id) {
    std::cout << "loading notebook " << notebook_id << std::endl;
    return std::make_unique<Notebook>();
}

void PgDatabase::newNote(Note &) {
    std::cout << "creating new note" << std::endl;
}

void PgDatabase::updateNote(const Note &note) {
    std::cout << "updateing note " << note.id() << std::endl;
}

void PgDatabase::addTag(const int note_id, const int tag_id) {
    std::cout << "adding tag " << tag_id << " to note " << note_id << std::endl;
}

void PgDatabase::removeTag(const int note_id, const int tag_id) {
    std::cout << "remove tag " << tag_id << " from note " << note_id
              << std::endl;
}

void PgDatabase::deleteNote(int id) {
    std::cout << "deleting note " << id << std::endl;
}

std::unique_ptr<Note> PgDatabase::loadNote(int note_id) {
    std::cout << "loading note " << note_id << std::endl;
    return std::make_unique<Note>();
}

int PgDatabase::newTag(const std::string &title) {
    std::cout << "creating new tag " << title << std::endl;
    return 0;
}

int PgDatabase::findTag(const std::string &title) {
    std::cout << "searching for tag " << title << std::endl;
    return 0;
}

void PgDatabase::deleteTag(const int tag_id) {
    std::cout << "deleting tag " << tag_id << std::endl;
}

std::vector<Note> PgDatabase::loadNotesFromNotebook(int notebook_id) {
    std::cout << "loadNotes: " << notebook_id << std::endl;
    return std::vector<Note>{};
}

std::vector<Note> PgDatabase::loadNotesForTag(int tag_id) {
    std::cout << "load notes for tag " << tag_id << std::endl;
    return std::vector<Note>{};
}

} // ns db
} // ns notes
