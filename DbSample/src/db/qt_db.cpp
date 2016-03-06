#include "db/qt_db.h"

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtCore/QVariantList>
#include <QtCore/QDateTime>

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
    QSqlQuery drop_tables;
    drop_tables.prepare("DROP TABLE IF EXISTS ?");
    QVariantList tabs;
    tabs << "tags_nm"
         << "tags"
         << "notes"
         << "notebooks";
    drop_tables.addBindValue(tabs);
    if (!drop_tables.execBatch())
        throw DatabaseException("Could not drop tables - error: " +
                                drop_tables.lastError().text().toStdString());

    QSqlQuery create_notebooks("CREATE TABLE notebooks ("
                               "id		serial primary key,"
                               "title	varchar(255)"
                               ")");
    if (!create_notebooks.exec())
        throw DatabaseException("Could not create table notebooks");

    QSqlQuery create_tags("CREATE TABLE tags ("
                          "id	 	serial primary key,"
                          "title	varchar(255)"
                          ")");
    if (!create_tags.exec())
        throw DatabaseException("Could not create table tags");

    QSqlQuery create_notes("CREATE TABLE notes ("
                           "id    	serial primary key,"
                           "title   	varchar(255),"
                           "content		text,"
                           "notebook 	int references notebooks(id),"
                           "last_change timestamp DEFAULT CURRENT_TIMESTAMP,"
                           "reminder	timestamp"
                           ")");
    if (!create_notes.exec())
        throw DatabaseException("Could not create table notes");

    QSqlQuery create_tags_nm("CREATE TABLE tags_nm ("
                             "tag_id		serial references tags(id),"
                             "note_id	serial references notes(id)"
                             ")");
    if (!create_tags_nm.exec())
        throw DatabaseException("Could not create table tags_nm");
}

// implementation of NotebookDatabase

// DEMO: result record handling
std::vector<Notebook> QtDatabase::listNotebooks()
{
    QSqlQuery q("SELECT * FROM notebooks");

    if (!q.isActive() || q.record().count() != 2)
        throw DatabaseException("listing notebooks failed, invalid result");

    std::vector<Notebook> result_vec;

    while (q.next()) {
        result_vec.emplace_back(q.value(0).toInt(),
                                q.value(1).toString().toStdString());
    }
    return result_vec;
}

int QtDatabase::newNotebook(const std::string &title)
{
    QSqlQuery q;
    q.prepare("INSERT INTO notebooks VALUES(:title)");
    q.bindValue(":title", QString::fromStdString(title));
    if (!q.exec())
        throw DatabaseException("Could not insert new notebook");
    QSqlQuery res(
        "SELECT id FROM notebooks WHERE (title=:title) order by id desc");
    if (!q.isActive() || !res.first())
        throw DatabaseException("Error retrieving ID from inserted notebook");

    return q.value(0).toInt();
}

void QtDatabase::renameNotebook(const int notebook_id,
                                const std::string &new_title)
{
    QSqlQuery q;
    q.prepare("UPDATE notebooks SET (title=:title) WHERE id=:id");
    q.bindValue(":title", QString::fromStdString(new_title));
    q.bindValue(":id", notebook_id);

    if (!q.exec())
        throw DatabaseException("Error updating notebook name");
}

void QtDatabase::deleteNotebook(const int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM notebooks WHERE id=:id");
    q.bindValue(":id", id);

    if (!q.exec())
        throw DatabaseException("Error deleting notebook");
}

Notebook QtDatabase::loadNotebook(const int notebook_id)
{
    QSqlQuery q;
    q.prepare("SELECT * FROM notebooks WHERE (id=:id)");
    q.bindValue(":id", notebook_id);

    if (!q.exec())
        throw DatabaseException("Could not execute notebook select query");
    if (!q.isActive() || !q.first())
        throw DatabaseException("Loading notebook select result failed");

    return Notebook(q.value(0).toInt(), q.value(1).toString().toStdString());
}

// DEMO : prepare / bind and generic result ID query
void QtDatabase::newNote(Note &note)
{
    QSqlQuery q;
    q.prepare(
        "INSERT INTO notes VALUES(:title, :content, :notebook, :reminder)");
    q.bindValue(":title", QString::fromStdString(note.title()));
    q.bindValue(":content", QString::fromStdString(note.content()));
    q.bindValue(":notebook", note.notebook());
    q.bindValue(":reminder", QDateTime::fromTime_t(static_cast<unsigned>(
                                 pt::to_time_t(note.reminder()))));
    if (!q.exec())
        throw DatabaseException("Could not insert new note into notes");
    QSqlQuery res;
    res.prepare("SELECT id FROM notes where (title=:title) order by id desc");
    res.bindValue(":title", QString::fromStdString(note.title()));
    if (!res.exec() || !res.first())
        throw DatabaseException("Error loading new note ID from notes");

    note.id(res.value(0).toInt());
}

void QtDatabase::updateNote(const Note &note)
{
    QSqlQuery q;
    q.prepare("UPDATE notes set (title=:title, content=:content,"
              "notebook=:nb_id, last_change=CURRENT_TIMESTAMP,"
              "reminder=:reminder) WHERE (id=:id)");
    q.bindValue(":title", QString::fromStdString(note.title()));
    q.bindValue(":content", QString::fromStdString(note.content()));
    q.bindValue(":notebook", note.notebook());
    q.bindValue(":reminder", QDateTime::fromTime_t(static_cast<unsigned>(
                                 pt::to_time_t(note.reminder()))));
    q.bindValue(":id", note.id());
    if (!q.exec())
        throw DatabaseException("Error updating note");
}

void QtDatabase::addTag(const int note_id, const int tag_id)
{
    QSqlQuery q;
    q.prepare("INSERT INTO tags_nm VALUES (:tag_id, :note_id)");
    q.bindValue(":note_id", note_id);
    q.bindValue(":tag_id", tag_id);
    if (!q.exec())
        throw DatabaseException("Error adding tag to note");
}

std::vector<model::Tag> db::QtDatabase::listTags()
{
    QSqlQuery q("SELECT * FROM tags");

    if (!q.isActive() || q.record().count() != 2)
        throw DatabaseException("listing notebooks failed, invalid result");

    std::vector<Tag> result_vec;

    while (q.next()) {
        result_vec.emplace_back(q.value(0).toInt(),
                                q.value(1).toString().toStdString());
    }
    return result_vec;
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
    QSqlQuery q;
    q.prepare("SELECT notes.id, notes.title, notes.content, notes.last_change, "
              "notes.reminder "
              "FROM notes LEFT JOIN notebooks ON (notes.notebook=notebooks.id) "
              "WHERE(notebooks.id=:id");
    q.bindValue(":id", notebook_id);

    if (!q.exec())
        throw DatabaseException("Executing select notes for notebook failed");
    if (!q.isActive() || q.record().count() != 5)
        throw DatabaseException(
            "listing notes by notebook failed, invalid result");

    std::vector<Note> result_vec;

    while (q.next()) {
        result_vec.emplace_back(
            q.value(0).toInt(), q.value(1).toString().toStdString(),
            q.value(2).toString().toStdString(), notebook_id,
            pt::from_time_t(q.value(3).toDateTime().toTime_t()),
            pt::from_time_t(q.value(4).toDateTime().toTime_t()));
    }
    return result_vec;
}

std::vector<Note> QtDatabase::loadNotesForTag(const int tag_id)
{
    QSqlQuery q;
    q.prepare("SELECT notes.id, notes.title, notes.content, notes.notebook, "
              "notes.last_change, "
              " notes.reminder FROM notes LEFT JOIN tags_nm ON "
              "(notes.id=tags_nm.note_id) "
              " WHERE(tags_nm.tag_id=:id) ORDER BY notes.id ASC");
    q.bindValue(":id", tag_id);

    if (!q.exec())
        throw DatabaseException("Executing select notes for tag failed");
    if (!q.isActive() || q.record().count() != 6)
        throw DatabaseException("Listing notes by tag failed, invalid result");

    std::vector<Note> result_vec;

    while (q.next()) {
        result_vec.emplace_back(
            q.value(0).toInt(), q.value(1).toString().toStdString(),
            q.value(2).toString().toStdString(), q.value(3).toInt(),
            pt::from_time_t(q.value(4).toDateTime().toTime_t()),
            pt::from_time_t(q.value(5).toDateTime().toTime_t()));
    }
    return result_vec;
}

std::vector<Note> QtDatabase::searchNotes(const std::string &term)
{
    QSqlQuery q;
    q.prepare("SELECT notes.id, notes.title, notes.content, notes.notebook, "
              "notes.last_change, "
              " notes.reminder FROM notes LEFT JOIN tags_nm ON "
              "(notes.id=tags_nm.note_id) "
              " LEFT JOIN tags ON (tags_nm.tag_id=tags.id) WHERE ("
              " (notes.title ILIKE :term OR notes.content ILIKE :term OR "
              " tags.title ILIKE :term) ORDER BY notes.id ASC");
    q.bindValue(":term", QString::fromStdString("%" + term + "%"));

    if (!q.exec())
        throw DatabaseException("Executing search notes failed");
    if (!q.isActive() || q.record().count() != 6)
        throw DatabaseException("Searching for notes failed, invalid result");

    std::vector<Note> result_vec;

    while (q.next()) {
        result_vec.emplace_back(
            q.value(0).toInt(), q.value(1).toString().toStdString(),
            q.value(2).toString().toStdString(), q.value(3).toInt(),
            pt::from_time_t(q.value(4).toDateTime().toTime_t()),
            pt::from_time_t(q.value(5).toDateTime().toTime_t()));
    }
    return result_vec;
}

} // ns db
} // ns notes
