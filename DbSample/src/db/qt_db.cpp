#include "db/qt_db.h"

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtCore/QVariantList>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>
#include <QDebug>

#include <iostream>
#include <algorithm>
#include <string>

namespace notes
{
namespace db
{

using namespace model;
QCoreApplication *app;
constexpr static const char appname[] = "notebook";

QtDatabase::QtDatabase(const std::string &connection_info)
{
    int params = 0;
    char *args[1] = {const_cast<char *>(appname)};
    if (!app)
        app = new QCoreApplication(params, args);

    config_ = parseConnectionInfo(connection_info);

    // note: no connection name -> default connection
    // other connections could be retrieved using
    // QSqlDatabase::database(connectionName)
    QSqlDatabase db = QSqlDatabase::addDatabase(
        QString::fromStdString(config_.driver) /*, connectionName */);

    std::cout << "Connecting with driver " << config_.driver << " to "
              << config_.dbname << std::endl;
    db.setDatabaseName(QString::fromStdString(config_.dbname));
    db.setHostName(QString::fromStdString(config_.host));
    db.setPort(QString::fromStdString(config_.port).toShort());
    db.setUserName(QString::fromStdString(config_.username));
    db.setPassword(QString::fromStdString(config_.password));

    if (!db.open())
        throw DatabaseException(
            "Could not connect to database using QtSql:" +
            (QSqlDatabase::database().lastError().text().toStdString()));
}

QtDatabase::~QtDatabase() { QSqlDatabase::database().close(); }

static void throw_query(const std::string &msg, const QSqlQuery &q)
{
    throw DatabaseException(msg + ": " + q.lastError().text().toStdString());
}

void QtDatabase::setupDb()
{
    // "DROP TABLE IF EXISTS tags_nm, notes, tags, notebooks;"),
    QSqlQuery drop_tables;
    if (!drop_tables.exec("DROP TABLE IF EXISTS tags_nm"))
        throw DatabaseException("Could not drop tables - error: " +
                                drop_tables.lastError().text().toStdString());

    if (!drop_tables.exec("DROP TABLE IF EXISTS tags"))
        throw DatabaseException("Could not drop tables - error: " +
                                drop_tables.lastError().text().toStdString());

    if (!drop_tables.exec("DROP TABLE IF EXISTS notes"))
        throw DatabaseException("Could not drop tables - error: " +
                                drop_tables.lastError().text().toStdString());

    if (!drop_tables.exec("DROP TABLE IF EXISTS notebooks"))
        throw DatabaseException("Could not drop tables - error: " +
                                drop_tables.lastError().text().toStdString());

    std::string serial_type = "serial";
    if (QSqlDatabase::database().driverName() == "QSQLITE") {
        serial_type = "integer";
    }

    QSqlQuery create_notebooks;
    if (!create_notebooks.exec(("CREATE TABLE notebooks ("
                                "id		" +
                                serial_type + " primary key,"
                                              "title	varchar(255)"
                                              ")").c_str()))
        throw_query("Could not create table notebooks", create_notebooks);

    QSqlQuery create_tags;
    if (!create_tags.exec(("CREATE TABLE tags ("
                           "id	    " +
                           serial_type + " primary key,"
                                         "title	varchar(255)"
                                         ")").c_str()))
        throw_query("Could not create table tags", create_tags);

    QSqlQuery create_notes;
    if (!create_notes.exec(
            ("CREATE TABLE notes ("
             "id    	    " +
             serial_type +
             " primary key,"
             "title   	varchar(255),"
             "content		text,"
             "notebook 	int references notebooks(id) ON DELETE CASCADE,"
             "last_change timestamp DEFAULT CURRENT_TIMESTAMP,"
             "reminder	timestamp"
             ")").c_str()))
        throw_query("Could not create table notes", create_notes);

    QSqlQuery create_tags_nm;
    if (!create_tags_nm.exec(
            ("CREATE TABLE tags_nm ("
             "tag_id    " +
             serial_type + " references tags(id) ON DELETE CASCADE,"
                           "note_id	" +
             serial_type + " references notes(id) ON DELETE CASCADE"
                           ")").c_str()))
        throw_query("Could not create table tags_nm", create_tags_nm);
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

// DEMO: generic result ID query is not very nice ...
int QtDatabase::newNotebook(const std::string &title)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("INSERT INTO notebooks(title) VALUES( :title )");
    if (ok) {
        q.bindValue(":title", QString::fromStdString(title));
        ok = q.exec();
    }
    if (!ok)
        throw_query("Could not insert new notebook", q);

    QSqlQuery res;
    ok = res.prepare(
        "SELECT id FROM notebooks WHERE (title=:title) order by id desc");
    if (ok) {
        res.bindValue(":title", QString::fromStdString(title));
        ok = res.exec();
        if (ok)
            ok = res.first();
    }
    if (!ok)
        throw_query("Error retrieving ID from inserted notebook", res);

    return res.value(0).toInt();
}

void QtDatabase::renameNotebook(const int notebook_id,
                                const std::string &new_title)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("UPDATE notebooks SET title=:title WHERE (id=:id)");
    if (ok) {
        q.bindValue(":title", QString::fromStdString(new_title));
        q.bindValue(":id", notebook_id);

        ok = q.exec();
    }
    if (!ok)
        throw_query("Error updating notebook name", q);
}

void QtDatabase::deleteNotebook(const int id)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("DELETE FROM notebooks WHERE id=:id");
    if (ok) {
        q.bindValue(":id", id);
        ok = q.exec();
    }
    if (!ok)
        throw_query("Error deleting notebook", q);
}

Notebook QtDatabase::loadNotebook(const int notebook_id)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("SELECT * FROM notebooks WHERE (id=:id)");
    if (ok) {
        q.bindValue(":id", notebook_id);

        ok = q.exec();
        if (ok)
            ok = q.first();
    }
    if (!ok)
        throw_query("Could not execute notebook select query", q);
    return Notebook(q.value(0).toInt(), q.value(1).toString().toStdString());
}

// DEMO : prepare / bind and generic result ID query
void QtDatabase::newNote(Note &note)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("INSERT INTO notes(title,content,notebook,reminder)"
                   "VALUES(:title, :content, :notebook, :reminder)");
    if (ok) {
        q.bindValue(":title", QString::fromStdString(note.title()));
        q.bindValue(":content", QString::fromStdString(note.content()));
        q.bindValue(":notebook", note.notebook());
        q.bindValue(":reminder", QDateTime::fromTime_t(static_cast<unsigned>(
                                     pt::to_time_t(note.reminder()))));
        ok = q.exec();
        if (ok) {
            QSqlQuery res;
            ok = res.prepare(
                "SELECT id FROM notes where (title=:title) order by id desc");
            res.bindValue(":title", QString::fromStdString(note.title()));
            if (ok)
                ok = res.exec();
            if (ok)
                ok = res.first();
            if (ok)
                note.id(res.value(0).toInt());
        }
    }
    if (!ok)
        throw_query("Could not insert new note into notes", q);
}

void QtDatabase::updateNote(const Note &note)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("UPDATE notes SET title=:title, content=:content,"
                   "notebook=:nb_id, last_change=CURRENT_TIMESTAMP,"
                   "reminder=:reminder WHERE (id=:id)");
    if (ok) {
        q.bindValue(":title", QString::fromStdString(note.title()));
        q.bindValue(":content", QString::fromStdString(note.content()));
        q.bindValue(":nb_id", note.notebook());
        q.bindValue(":reminder", QDateTime::fromTime_t(static_cast<unsigned>(
                                     pt::to_time_t(note.reminder()))));
        q.bindValue(":id", note.id());
        if (ok)
            ok = q.exec();
    }
    if (!ok)
        throw_query("Error updating note", q);
}

void QtDatabase::addTag(const int note_id, const int tag_id)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("INSERT INTO tags_nm VALUES (:tag_id, :note_id)");
    if (ok) {
        q.bindValue(":note_id", note_id);
        q.bindValue(":tag_id", tag_id);
        if (ok)
            ok = q.exec();
    }
    if (!ok)
        throw_query("Error adding tag to note", q);
}

// DEMO : query in constructor
std::vector<model::Tag> db::QtDatabase::listTags()
{
    QSqlQuery q("SELECT * FROM tags");

    if (!q.isActive() || q.record().count() != 2)
        throw_query("listing notebooks failed, invalid result", q);

    std::vector<Tag> result_vec;

    while (q.next()) {
        result_vec.emplace_back(q.value(0).toInt(),
                                q.value(1).toString().toStdString());
    }
    return result_vec;
}

void QtDatabase::removeTag(const int note_id, const int tag_id)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare(
        "DELETE FROM tags_nm WHERE (note_id=:note_id and tag_id=:tag_id)");
    if (ok) {
        q.bindValue(":note_id", note_id);
        q.bindValue(":tag_id", tag_id);
        ok = q.exec();
    }
    if (!ok)
        throw_query("Error deleting tag", q);
}

void QtDatabase::deleteNote(const int id)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("DELETE FROM notes WHERE (id=:id)");
    if (ok) {
        q.bindValue(":id", id);
        ok = q.exec();
    }
    if (!ok)
        throw_query("Error deleting note", q);
}

Note QtDatabase::loadNote(const int note_id)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("SELECT * FROM notes WHERE (id=:id)");
    if (ok) {
        q.bindValue(":id", note_id);

        ok = q.exec();
        if (ok)
            ok = q.first();
    }
    if (!ok)
        throw_query("Could not execute note select query", q);

    return Note(q.value(0).toInt(), q.value(1).toString().toStdString(),
                q.value(2).toString().toStdString(), q.value(3).toInt(),
                pt::from_time_t(q.value(4).toDateTime().toTime_t()),
                pt::from_time_t(q.value(5).toDateTime().toTime_t()));
}

int QtDatabase::newTag(const std::string &title)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("INSERT INTO tags(title) VALUES( :title )");
    if (ok) {
        q.bindValue(":title", QString::fromStdString(title));
        ok = q.exec();
    }
    if (!ok)
        throw_query("Could not insert new tag", q);

    QSqlQuery res;
    ok = res.prepare(
        "SELECT id FROM tags WHERE (title=:title) order by id desc");
    if (ok) {
        res.bindValue(":title", QString::fromStdString(title));
        ok = res.exec();
        if (ok)
            ok = res.first();
        if (ok)
            return res.value(0).toInt();
    }
    if (!ok)
        throw_query("Error retrieving ID from inserted tag", res);
    return -1;
}

void QtDatabase::deleteTag(const int tag_id)
{
    bool ok{true};
    QSqlQuery q;
    ok = q.prepare("DELETE FROM tags WHERE (id=:id)");
    if (ok) {
        q.bindValue(":id", tag_id);
        ok = q.exec();
    }
    if (!ok)
        throw_query("Error deleting tag", q);
}

std::vector<Note> QtDatabase::loadNotesFromNotebook(const int notebook_id)
{
    bool ok{true};
    std::vector<Note> result_vec;

    QSqlQuery q;
    ok = q.prepare(
        "SELECT notes.id, notes.title, notes.content, notes.last_change, "
        "notes.reminder FROM notes LEFT JOIN notebooks "
        "ON (notes.notebook=notebooks.id) WHERE (notebooks.id=:id)");

    if (ok) {
        q.bindValue(":id", notebook_id);

        ok = q.exec();
        if (ok)
            ok = q.isActive() && q.record().count() == 5;

        while (ok && q.next()) {
            result_vec.emplace_back(
                q.value(0).toInt(), q.value(1).toString().toStdString(),
                q.value(2).toString().toStdString(), notebook_id,
                pt::from_time_t(q.value(3).toDateTime().toTime_t()),
                pt::from_time_t(q.value(4).toDateTime().toTime_t()));
        }
    }
    if (!ok)
        throw_query("Executing select notes for notebook failed", q);

    return result_vec;
}

std::vector<Note> QtDatabase::loadNotesForTag(const int tag_id)
{
    QSqlQuery q;
    q.prepare("SELECT notes.id, notes.title, notes.content, notes.notebook, "
              "notes.last_change, notes.reminder FROM notes "
              "LEFT JOIN tags_nm ON (notes.id=tags_nm.note_id) "
              "WHERE(tags_nm.tag_id=:id) ORDER BY notes.id ASC");
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
    bool ok{true};
    std::vector<Note> result_vec;

    QSqlQuery q;
    ok = q.prepare(
        "SELECT notes.id, notes.title, notes.content, notes.notebook, "
        "notes.last_change, notes.reminder FROM notes "
        "LEFT JOIN tags_nm ON (notes.id=tags_nm.note_id) "
        "LEFT JOIN tags ON (tags_nm.tag_id=tags.id) WHERE "
        "(notes.title ILIKE :term OR notes.content ILIKE :term OR "
        "tags.title ILIKE :term) ORDER BY notes.id ASC");
    if (ok) {
        q.bindValue(":term", QString::fromStdString("%" + term + "%"));

        ok = q.exec();
        if (ok)
            ok = (q.isActive() && q.record().count() == 6);

        while (q.next()) {
            result_vec.emplace_back(
                q.value(0).toInt(), q.value(1).toString().toStdString(),
                q.value(2).toString().toStdString(), q.value(3).toInt(),
                pt::from_time_t(q.value(4).toDateTime().toTime_t()),
                pt::from_time_t(q.value(5).toDateTime().toTime_t()));
        }
    }
    if (!ok)
        throw_query("Searching for notes failed", q);
    return result_vec;
}

} // ns db
} // ns notes
