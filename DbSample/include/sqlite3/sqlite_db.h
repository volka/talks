#pragma once

#include "db_facade.h"

#include <sqlite3.h>

namespace notes {
namespace db {

// sqlite_conn_ptr is finalized by sqlite3_close()
class sqlite_conn {
    sqlite3 *const conn_;

  public:
    sqlite_conn(sqlite3 *const conn) : conn_(conn) {}

    ~sqlite_conn() {
        int result = sqlite3_close_v2(conn_);
        if (result != SQLITE_OK) {
            throw DatabaseException("Error closing Sqlite connection, ec=" +
                                    std::to_string(result));
        }
    }

    inline sqlite3 *const ptr() const { return conn_; }
};

// an sqlite Statement obtained sqlite3_prepare, needs to be freed using
// sqlite3_finalize
class sqlite_stmt {
    sqlite3_stmt *const stmt_;

  public:
    sqlite_stmt(sqlite3_stmt *const stmt) : stmt_(stmt) {}

    ~sqlite_stmt() {
        int result = sqlite3_finalize(stmt_);
        if (result != SQLITE_OK) {
            throw DatabaseException(
                "Error finalizing Sqlite Statement pointer, ec=" +
                std::to_string(result));
        }
    }

    inline sqlite3_stmt *const ptr() const { return stmt_; }
};

// Implementation of the Notebook DB interface for SQLite3
class Sqlite3Database : public NotebookDatabase {

  private:
    // filename or ":memory:"
    std::string connection_info_;
    // sqlite uses plain ints as connection handles
    sqlite_conn_ptr connection_;

  public:
    Sqlite3Database(const Sqlite3Database &) = delete;

    Sqlite3Database(const std::string &connection_info);

    virtual ~Sqlite3Database();

    // helper functions to initialize DB and seed with some sample data
    virtual void setupDb() override;
    virtual void fillDb() override;

    // create a notebook, return the generated ID
    virtual std::vector<Notebook> listNotebooks() override;
    virtual int newNotebook(const std::string &title) override;
    virtual void renameNotebook(const int notebook_id,
                                const std::string &new_title) override;
    virtual void deleteNotebook(const int id) override;
    virtual Notebook loadNotebook(const int notebook_id) override;

    // create a new note
    virtual void newNote(Note &) override;
    virtual void updateNote(const Note &note) override;
    virtual void addTag(const int note_id, const int tag_id) override;
    virtual void removeTag(const int note_id, const int tag_id) override;
    virtual void deleteNote(int id) override;
    virtual Note loadNote(int note_id) override;

    virtual int newTag(const std::string &title) override;
    virtual int findTag(const std::string &title) override;
    virtual void deleteTag(const int tag_id) override;

    virtual std::vector<Note> loadNotesFromNotebook(int notebook_id) override;
    virtual std::vector<Note> loadNotesForTag(int tag_id) override;

  private:
    // retrieve the last inserted row-id (usually the autoincrement ID)
    uint64_t getLastInsertId();
    sqlite_stmt_ptr prepareStatement(const std::string &stmt);
    bool executeStatement(sqlite_stmt_ptr &);
    // simple check if an error was returned
    bool checkResult(int result, int expected,
                     const std::string &msg = "Command failed",
                     bool do_throw = false);
};

} // db
} // notes
