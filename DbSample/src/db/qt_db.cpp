#include "db/qt_db.h"

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <iostream>
#include <algorithm>
#include <string>

namespace notes
{
namespace db
{

using namespace model;

QtDatabase::QtDatabase(const std::string &connection_info)
{
    config_ = parseConnectionInfo(connection_info);

    // note: no connection name -> default connection
    // other connections could be retrieved using
    // QSqlDatabase::database(connectionName)
    QSqlDatabase db = QSqlDatabase::addDatabase(
        QString::fromStdString(config_.driver) /*, connectionName */);

    db.setDatabaseName(QString::fromStdString(config_.dbname));
    db.setHostName(QString::fromStdString(config_.host));
    db.setPort(QString::fromStdString(config_.port).toShort());
    db.setUserName(QString::fromStdString(config_.username));
    db.setPassword(QString::fromStdString(config_.password));

    if (!db.open())
        throw DatabaseException("Could not connect to database using QtSql");
}

QtDatabase::~QtDatabase() { QSqlDatabase::database().close(); }

void QtDatabase::setupDb()
{

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

void QtDatabase::fillDb()
{
    // note: here we use prepared statements and execBatch
}

std::vector<Notebook> QtDatabase::listNotebooks()
{
    return std::vector<Notebook>{};
}

int QtDatabase::newNotebook(const std::string &title)
{
    std::cout << title << std::endl;
    return 0;
}

void QtDatabase::renameNotebook(const int notebook_id,
                                const std::string &new_title)
{
    std::cout << notebook_id << new_title << std::endl;
    return;
}

void QtDatabase::deleteNotebook(const int id)
{
    std::cout << id << std::endl;
    return;
}

Notebook QtDatabase::loadNotebook(const int notebook_id)
{
    std::cout << notebook_id << std::endl;
    return Notebook(0, "default");
}

void QtDatabase::newNote(Note &) { return; }

void QtDatabase::updateNote(const Note &) { return; }

void QtDatabase::addTag(const int note_id, const int tag_id)
{
    std::cout << "addTag: " << note_id << " " << tag_id << std::endl;
    return;
}

void QtDatabase::removeTag(const int note_id, const int tag_id)
{
    std::cout << "removeTag: " << note_id << " " << tag_id << std::endl;
    return;
}

void QtDatabase::deleteNote(const int id)
{
    std::cout << "deleteNote " << id << std::endl;
    return;
}

Note QtDatabase::loadNote(const int note_id)
{
    std::cout << "loadNote " << note_id << std::endl;
    return Note(-1, "foo", "bar", 0, pt::ptime(), pt::ptime());
}

int QtDatabase::newTag(const std::string &title)
{
    std::cout << "newTag " << title << std::endl;
    return 0;
}

void QtDatabase::deleteTag(const int tag_id)
{
    std::cout << "deleteTag" << tag_id << std::endl;
    return;
}

std::vector<Note> QtDatabase::loadNotesFromNotebook(const int notebook_id)
{
    std::cout << "loadNotesFromNotebook " << notebook_id << std::endl;
    return std::vector<Note>{};
}

std::vector<Note> QtDatabase::loadNotesForTag(const int tag_id)
{
    std::cout << "loadNotesForTag " << tag_id << std::endl;
    return std::vector<Note>{};
}

} // ns db
} // ns notes
