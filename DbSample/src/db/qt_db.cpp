#include "qtsql/qt_db.h"

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <iostream>
#include <algorithm>
#include <string>

namespace notes {
namespace db {

using namespace model;

namespace {

using namespace std;

// first parse the connection string to suite the QtSQL format

// this parses the PSQL specific fields
void parsePgConfig(const std::string &conn_info,
                   QtDatabase::ConnectionConfig &config) {
    // parse dbname, host, username, password - separated by spaces
    auto space_it = conn_info.begin(), end = conn_info.end();
    string option, value;
    while (space_it != end) {

        option.clear();
        value.clear();

        auto eq_it = find(space_it, end(conn_info), "=");
        if (eq_it == end(conn_info))
            return;
        option.insert(space_it, eq_it);

        // end of value, also next option ...
        space_it = find(begin(conn_info), end(conn_info), " ");
        value.insert(eq_it++, space_it);

        if (space_it != end(conn_info))
            ++space_it;

        if (option == "dbname")
            config.dbname = value;
        else if (option == "host")
            config.host = value;
        else if (option == "port")
            config.port = value;
        else if (option == "username")
            config.username = value;
        else if (option == "password")
            config.password = value;
        else
            throw DatabaseException("Invalid option in connection_info: " +
                                    option);
    }
    return;
}

// parse the connection_info for QSQL to separate the DRIVER and Parameters
QtDatabase::ConnectionConfig parseConnectionInfo(const std::string &conn_info) {
    QtDatabase::ConnectionConfig config;

    size_t split = find(begin(connection_info), end(connection_info), ":");

    if (split != string::npos) {
        config.driver = connection_info.substr(0, split);

        if (config.driver == "QSQLITE") {
            config.dbname = connection_info.substr(split, end(connection_info));
            if (config.dbname.empty())
                throw DatabaseException(
                    "Invalid DB name for QtSql Sqlite connection");
        } else if (config.driver == "QPSQL") {
            parsePgConfig(connection_info.substr(split, end(connection_info)),
                          config);
        } else {
            throw DatabaseException("Unsupported driver for QSQL: " +
                                    config.driver);
        }
    } else
        throw DatabaseException(
            "Invalid connection string, could not find ':'");

    return config;
}
}

QtDatabase::QtDatabase(const std::string &connection_info) {
    config_ = parseConnectionInfo(connection_info);

    // note: no connection name -> default connection
    // other connections could be retrieved using
    // QSqlDatabase::database(connectionName)
    QSqlDatabase db =
        QSqlDatabase::addDatabase(config_.driver /*, connectionName */);

    db.setDatabaseName(config_.dbname);
    db.setHostName(config_.host);
    db.setPort(config_.port);
    db.setUserName(config_.username);
    db.setPassword(config_.password);

    if (!db.open())
        throw DatabaseException("Could not connect to database using QtSql");
}

QtDatabase::~QtDatabase() { QSqlDatabase::close(); }

void QtDatabase::setupDb() {

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
}

void QtDatabase::fillDb() {
    // note: here we use prepared statements and execBatch
}

std::vector<Notebook> QtDatabase::listNotebooks() {
    return std::vector<Notebook>{};
}

int QtDatabase::newNotebook(const std::string &title) {
    std::cout << title << std::endl;
    return 0;
}

void QtDatabase::renameNotebook(const int notebook_id,
                                const std::string &new_title) {
    std::cout << notebook_id << new_title << std::endl;
    return;
}

void QtDatabase::deleteNotebook(const int id) {
    std::cout << id << std::endl;
    return;
}

Notebook QtDatabase::loadNotebook(const int notebook_id) {
    std::cout << notebook_id << std::endl;
    return Notebook{};
}

void QtDatabase::newNote(Note &) { return; }

void QtDatabase::updateNote(const Note &) { return; }

void QtDatabase::addTag(const int note_id, const int tag_id) {
    std::cout << "addTag: " << note_id << " " << tag_id << std::endl;
    return;
}

void QtDatabase::removeTag(const int note_id, const int tag_id) {
    std::cout << "removeTag: " << note_id << " " << tag_id << std::endl;
    return;
}

void QtDatabase::deleteNote(int id) {
    std::cout << "deleteNote " << id << std::endl;
    return;
}

Note QtDatabase::loadNote(int note_id) {
    std::cout << "loadNote " << note_id << std::endl;
    return Note{};
}

int QtDatabase::newTag(const std::string &title) {
    std::cout << "newTag " << title << std::endl;
    return 0;
}

int QtDatabase::findTag(const std::string &title) {
    std::cout << "findTag " << title << std::endl;
    return 0;
}

void QtDatabase::deleteTag(const int tag_id) {
    std::cout << "deleteTag" << tag_id << std::endl;
    return;
}

std::vector<Note> QtDatabase::loadNotesFromNotebook(int notebook_id) {
    std::cout << "loadNotesFromNotebook " << notebook_id << std::endl;
    return std::vector<Note>{};
}

std::vector<Note> QtDatabase::loadNotesForTag(int tag_id) {
    std::cout << "loadNotesForTag " << tag_id << std::endl;
    return std::vector<Note>{};
}

} // ns db
} // ns notes
