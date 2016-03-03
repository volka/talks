#include "db/qt_db.h"

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
    size_t space_pos = 0;
    string option, value;
    while (space_pos != string::npos) {

        option.clear();
        value.clear();

        auto eq_pos = conn_info.find("=", space_pos);
        if (eq_pos == string::npos)
            return;
        option = conn_info.substr(space_pos, eq_pos - space_pos);

        // end of value, also next option ...
        space_pos = conn_info.find(" ", eq_pos);
        if (space_pos == string::npos)
            space_pos = conn_info.size();
        ++eq_pos;

        value = conn_info.substr(eq_pos, space_pos - eq_pos);

        if (space_pos != string::npos)
            ++space_pos;

        std::cout << "option: " << option << " val " << value;
        if (option == "dbname")
            config.dbname = QString::fromStdString(value);
        else if (option == "host")
            config.host = QString::fromStdString(value);
        else if (option == "port")
            config.port = QString::fromStdString(value);
        else if (option == "username")
            config.username = QString::fromStdString(value);
        else if (option == "password")
            config.password = QString::fromStdString(value);
        else
            throw DatabaseException("Invalid option in connection_info: " +
                                    option);
    }
    return;
}

// parse the connection_info for QSQL to separate the DRIVER and Parameters
QtDatabase::ConnectionConfig parseConnectionInfo(const std::string &conn_info) {
    QtDatabase::ConnectionConfig config;

    size_t split = conn_info.find(":");

    if (split != string::npos) {
        config.driver = QString::fromStdString(conn_info.substr(0, split));
        ++split;
        if (config.driver == "QSQLITE") {
            config.dbname = QString::fromStdString(
                conn_info.substr(split, conn_info.size() - split));
            if (config.dbname.isEmpty())
                throw DatabaseException(
                    "Invalid DB name for QtSql Sqlite connection");
        } else if (config.driver == "QPSQL") {
            parsePgConfig(conn_info.substr(split, conn_info.size() - split),
                          config);
        } else {
            throw DatabaseException("Unsupported driver for QSQL: " +
                                    config.driver.toStdString());
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
    db.setPort(config_.port.toShort());
    db.setUserName(config_.username);
    db.setPassword(config_.password);

    if (!db.open())
        throw DatabaseException("Could not connect to database using QtSql");
}

QtDatabase::~QtDatabase() { QSqlDatabase::database().close(); }

void QtDatabase::setupDb() {

    // "DROP TABLE IF EXISTS tags_nm, notes, tags, notebooks;"),

    // "CREATE TABLE notebooks ("
    //                          "id		serial primary key,"
    //                          "title	varchar(255)"
    //                          ")"
    // "CREATE TABLE tags ("
    //                     "id	 	serial primary key,"
    //                     "title	varchar(255)"
    //                     ")"

    // "CREATE TABLE notes ("
    //                      "id    	serial primary key,"
    //                      "title   	varchar(255),"
    //                      "content		text,"
    //                      "notebook 	int references notebooks(id),"
    //                      "last_change timestamp DEFAULT CURRENT_TIMESTAMP,"
    //                      "reminder	timestamp"
    //                      ")"
    //
    // "CREATE TABLE tags_nm ("
    //                        "tag_id		serial references tags(id),"
    //                        "note_id	serial references notes(id)"
    //                      ")"
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
