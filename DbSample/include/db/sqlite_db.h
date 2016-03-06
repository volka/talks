#pragma once

#include "db_facade.h"

#include <sqlite3.h>

namespace notes
{
namespace db
{

// sqlite_conn_ptr is finalized by sqlite3_close()
class sqlite_conn
{
    sqlite3 *conn_;

  public:
    sqlite_conn(sqlite3 *conn) : conn_(conn) {}
    sqlite_conn(sqlite_conn &) = delete;
    sqlite_conn(sqlite_conn &&other) { std::swap(conn_, other.conn_); }
    sqlite_conn &operator=(sqlite_conn &) = delete;
    sqlite_conn &operator=(sqlite_conn &&other)
    {
        std::swap(conn_, other.conn_);
        return *this;
    }

    ~sqlite_conn()
    {
        if (conn_) {
            int result = sqlite3_close_v2(conn_);
            if (result != SQLITE_OK) {
                std::cout << "Error closing Sqlite connection, ec="
                          << std::to_string(result) << std::endl;
            }
        }
    }

    inline sqlite3 *ptr() const { return conn_; }
};

// an sqlite Statement obtained sqlite3_prepare, needs to be freed using
// sqlite3_finalize
class sqlite_stmt
{
    sqlite3_stmt *stmt_;

  public:
    sqlite_stmt(sqlite3_stmt *stmt) : stmt_(stmt) {}
    sqlite_stmt(sqlite_stmt &) = delete;
    sqlite_stmt(sqlite_stmt &&other) { std::swap(stmt_, other.stmt_); }
    sqlite_stmt &operator=(sqlite_stmt &) = delete;
    sqlite_stmt &operator=(sqlite_stmt &&other)
    {
        std::swap(stmt_, other.stmt_);
        return *this;
    }

    ~sqlite_stmt()
    {
        int result = sqlite3_finalize(stmt_);
        if (result != SQLITE_OK) {
            std::cout << "Error finalizing Sqlite Statement pointer, ec="
                      << std::to_string(result) << std::endl;
        }
    }

    inline sqlite3_stmt *ptr() const { return stmt_; }
};

// Implementation of the Notebook DB interface for SQLite3
class Sqlite3Database : public NotebookDatabase
{

  private:
    // filename or ":memory:"
    std::string connection_info_;
    // sqlite uses plain ints as connection handles
    sqlite_conn connection_;
    std::stringstream stmt_cache_;

  public:
    Sqlite3Database(const Sqlite3Database &) = delete;

    Sqlite3Database(const std::string &connection_info);

    virtual ~Sqlite3Database();

    // helper functions to initialize DB and seed with some sample data
    virtual void setupDb() override;

    // create a notebook, return the generated ID
    virtual std::vector<Notebook> listNotebooks() override;
    virtual int newNotebook(const std::string &title) override;
    virtual void renameNotebook(const int notebook_id,
                                const std::string &new_title) override;
    virtual void deleteNotebook(const int notebook_id) override;
    virtual Notebook loadNotebook(const int notebook_id) override;

    // create a new note
    virtual void newNote(Note &note) override;
    virtual void updateNote(const Note &note) override;
    virtual void addTag(const int note_id, const int tag_id) override;
    virtual void removeTag(const int note_id, const int tag_id) override;
    virtual void deleteNote(const int note_id) override;
    virtual Note loadNote(const int note_id) override;

    virtual int newTag(const std::string &title) override;
    virtual std::vector<Tag> listTags() override;
    virtual void deleteTag(const int tag_id) override;

    virtual std::vector<Note> loadNotesFromNotebook(int notebook_id) override;
    virtual std::vector<Note> loadNotesForTag(int tag_id) override;

    virtual std::vector<Note> searchNotes(const std::string &term) override;

  private:
    // retrieve the last inserted row-id (usually the autoincrement ID)
    int getLastInsertId();
    sqlite_stmt prepareStatement(const std::string &stmt);
    int executeStep(sqlite_stmt &);
    // simple check if an error was returned
    bool checkResult(int result, int expected = SQLITE_OK,
                     const std::string &msg = "Command failed",
                     bool do_throw = false) const;
    bool isError(const int res) const;
    inline void clearStatement() { stmt_cache_.str(std::string()); }
    // note the missing
    int bindString(sqlite_stmt &stmt, const int pos, const std::string &str);
    int bindInt(sqlite_stmt &stmt, const int pos, const int val);
    int getInt(const sqlite_stmt &stmt, const int column);
    std::string getString(const sqlite_stmt &stmt, const int column);
    pt::ptime getTimestamp(const sqlite_stmt &stmt, const int column);
};

} // db
} // notes
