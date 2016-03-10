#pragma once

#include <exception>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "model/note.h"
#include "model/notebook.h"
#include "model/tag.h"

namespace notes
{

namespace db
{

using namespace notes::model;

using bigint_t = long long;

// exceptions...
class DatabaseException : public std::domain_error
{
    using std::domain_error::domain_error;
};

// abstract base class for Notebook Database interface, used for the GUI
class NotebookDatabase
{
  public:
    // constants for database types
    struct Types
    {
        static constexpr auto sqlite = "sqlite";
        static constexpr auto postgres = "postgres";
        static constexpr auto qtsql = "qtsql";
        static constexpr auto wt_db = "wtdbo";
        static constexpr auto sqlpp = "sqlpp";
    };

    struct ConnectionConfig
    {
        std::string driver; // for QtSql QPSQL, QSQLITE, for Wt::Dbo SQLITE / PG
        std::string dbname; // file for SQLITE, database name for PG
        std::string host;
        std::string port;
        std::string username;
        std::string password;
        std::string conn_str; // copy of complete string, only excluding driver
    };

    // factory for DB connections, call with Db type
    static std::unique_ptr<NotebookDatabase> create(const std::string &type,
                                                    const std::string &config);

    virtual ~NotebookDatabase();

    // helper functions to initialize DB and seed with some sample data
    virtual void setupDb() = 0;
    virtual void fillDb();

    // create a notebook, return the generated ID
    virtual std::vector<Notebook> listNotebooks() = 0;
    virtual bigint_t newNotebook(const std::string &title) = 0;
    virtual void renameNotebook(const bigint_t notebook_id,
                                const std::string &new_title) = 0;
    virtual void deleteNotebook(const bigint_t id) = 0;
    virtual Notebook loadNotebook(const bigint_t notebook_id) = 0;

    // create a new note
    virtual void newNote(Note &) = 0;
    virtual void updateNote(const Note &) = 0;
    virtual void addTag(const bigint_t note_id, const bigint_t tag_id) = 0;
    virtual void removeTag(const bigint_t note_id, const bigint_t tag_id) = 0;
    virtual void deleteNote(const bigint_t id) = 0;
    virtual Note loadNote(const bigint_t note_id) = 0;

    virtual bigint_t newTag(const std::string &title) = 0;
    virtual std::vector<Tag> listTags() = 0;
    virtual void deleteTag(const bigint_t tag_id) = 0;

    virtual std::vector<Note>
    loadNotesFromNotebook(const bigint_t notebook_id) = 0;
    virtual std::vector<Note> loadNotesForTag(const bigint_t tag_id) = 0;

    // find notes by term in title, content or tags
    virtual std::vector<Note> searchNotes(const std::string &term) = 0;
};

// for parsing the connection string wiht QtSql / Wt::Dbo
NotebookDatabase::ConnectionConfig
parseConnectionInfo(const std::string &conn_info);

} // db
} // notes
