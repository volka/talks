#include "sqlite3/sqlite_db.h"
#include <iostream>

namespace notes {
namespace db {

Sqlite3Database::Sqlite3Database(const std::string& connection_info)
    : connection_info_(connection_info)
{
    sqlite3* conn;
    int result = sqlite3_open(connection_info.c_str(), &conn);
    if (result == SQLITE_OK) {
        connection_ = sqlite_conn_ptr{conn, sqlite3_close};
    } else {
        throw DatabaseException("Error opening SQLite database " + connection_info
                                + " (error: " + std::to_string(result) + ")");
    }
}

Sqlite3Database::~Sqlite3Database()
{}

void Sqlite3Database::setupDb()
{
    auto result = prepareStatement("DROP TABLE IF EXISTS tags_nm;");
    if (!executeStatement(result))
        throw std::runtime_error("dropping table notebooks failed");

    result = prepareStatement("DROP TABLE IF EXISTS notes;");
    if (!executeStatement(result))
        throw std::runtime_error("dropping table notebooks failed");

    result = prepareStatement("DROP TABLE IF EXISTS tags;");
    if (!executeStatement(result))
        throw std::runtime_error("dropping table tags failed");

    result = prepareStatement("DROP TABLE IF EXISTS notebooks;");
    if (!executeStatement(result))
        throw std::runtime_error("dropping table tags failed");

    if (!executeStatement(result))
        throw std::runtime_error("dropping pre-existing tables failed");

    result = prepareStatement("CREATE TABLE notebooks ("
                              "id		integer primary key,"
                              "title	varchar(255)"
                              ")");

    if (!executeStatement(result))
        throw std::runtime_error("creating table notebooks failed");

    result = prepareStatement("CREATE TABLE tags ("
                              "id	 	integer primary key,"
                              "title	varchar(255)"
                              ")");

    if (!executeStatement(result))
        throw std::runtime_error("creating table tags failed");

    result = prepareStatement("CREATE TABLE notes ("
                              "id      	integer primary key,"
                              "title   	varchar(255),"
                              "content		text,"
                              "notebook 	int references notebooks(id),"
                              "last_change timestamp DEFAULT CURRENT_TIMESTAMP,"
                              "reminder	timestamp"
                              ")");

    if (!executeStatement(result))
        throw std::runtime_error("creating table notes failed");

    result = prepareStatement("CREATE TABLE tags_nm ("
                              "tag_id		integer references tags(id),"
                              "note_id	integer references notes(id)"
                              ")");

    if (!executeStatement(result))
        throw std::runtime_error("creating table tags_nm failed");
}

uint64_t Sqlite3Database::getLastInsertId()
{
    return sqlite3_last_insert_rowid(connection_.get());
}

sqlite_stmt_ptr Sqlite3Database::prepareStatement(const std::string &stmt)
{
    sqlite3_stmt* stmt_ptr;
    // connection, statement string, length, OUT stmt pointer, ignored "unused part of stmt"
    int res = sqlite3_prepare_v2(connection_.get(), stmt.c_str(), static_cast<int>(stmt.size()),
                                 &stmt_ptr, nullptr);
    checkResult(res, SQLITE_OK, "Error preparing statement " + stmt, true);
    return sqlite_stmt_ptr{stmt_ptr, sqlite3_finalize};
}

bool Sqlite3Database::executeStatement(sqlite_stmt_ptr& stmt)
{
    int res = sqlite3_step(stmt.get());
    switch (res) {
    case SQLITE_OK:
    case SQLITE_ROW:
    case SQLITE_DONE:
        return true;
    default:
        checkResult(res, SQLITE_OK, "Error executing statement");
        return false;
    }
}

bool Sqlite3Database::checkResult(int result, int expected = SQLITE_OK, const std::string& msg, bool do_throw )
{
        if (result != expected) {
            auto errmsg = msg + ": " + std::to_string(result) + " "
                    + std::string(sqlite3_errmsg(connection_.get()));
            if (do_throw) {
                throw DatabaseException(errmsg);
            } else {
                std::cout << errmsg << std::endl;
            }
            return false;
        }
        return true;
}

void Sqlite3Database::fillDb()
{

}

int Sqlite3Database::newNotebook(const std::string &title)
{
    std::cout << "new notebook " << title << std::endl;
    return 0;
}

void Sqlite3Database::renameNotebook(const int notebook_id, const std::string &new_title)
{

    std::cout << "rename notebook " << notebook_id << " to " << new_title << std::endl;
}

void Sqlite3Database::deleteNotebook(const int id)
{

    std::cout << "deleting notebook " << id << std::endl;
}

std::unique_ptr<Notebook> Sqlite3Database::loadNotebook(const int notebook_id)
{

    std::cout << "loading notebook " << notebook_id << std::endl;
    return std::make_unique<Notebook>();
}

void Sqlite3Database::newNote(Note &)
{

    std::cout << "creating new note" << std::endl;
}

void Sqlite3Database::updateNote(const Note & note)
{

    std::cout << "updateing note " << note.id() << std::endl;
}

void Sqlite3Database::addTag(const int note_id, const int tag_id)
{

    std::cout << "adding tag " << tag_id << " to note " << note_id << std::endl;
}

void Sqlite3Database::removeTag(const int note_id, const int tag_id)
{

    std::cout << "remove tag " << tag_id << " from note " << note_id << std::endl;
}

void Sqlite3Database::deleteNote(int id)
{
    std::cout << "delete note " << id << std::endl;
}

std::unique_ptr<Note> Sqlite3Database::loadNote(int note_id)
{
   std::cout << "load note " << note_id << std::endl;
   return nullptr;
}

int Sqlite3Database::newTag(const std::string &title)
{
   std::cout << "new tag " << title << std::endl;
   return 0;
}

int Sqlite3Database::findTag(const std::string &title)
{
   std::cout << "finding tag " << title << std::endl;
   return 0;
}

void Sqlite3Database::deleteTag(const int tag_id)
{
   std::cout << "deleting tag " << tag_id << std::endl;
}

std::vector<Note> Sqlite3Database::loadNotesFromNotebook(int notebook_id)
{

    std::cout << "load notes for notebook " << notebook_id << std::endl;
    return std::vector<Note>{};
}

std::vector<Note> Sqlite3Database::loadNotesForTag(int tag_id)
{
   std::cout << "load notes for tag " << tag_id << std::endl;
   return std::vector<Note>{};
}

} // ns db
} // ns notes
