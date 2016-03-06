#pragma once

#include "db_facade.h"
#include <libpq-fe.h>

namespace notes
{
namespace db
{

// wrap plain C pointers in RAII types...

// PGconn is finalized by PQfinish
using pg_conn_ptr = std::unique_ptr<PGconn, std::function<void(PGconn *)>>;
// a PGresult obtained e.g. through PQexec, needs to be freed using PQclear
using pg_result_ptr =
    std::unique_ptr<PGresult, std::function<void(PGresult *)>>;
// an esacped string as produced by PQescapeLiteral, free with PQfreemem
using pg_escaped_ptr = std::unique_ptr<char, std::function<void(void *)>>;

class PgDatabase : public NotebookDatabase
{
  private:
    std::string connection_info_;
    pg_conn_ptr connection_;
    std::stringstream stmt_cache_;
    std::string update_note_str_; // name of prepared statement

  public:
    PgDatabase(const PgDatabase &) = delete;
    PgDatabase(const std::string &connection_info);
    virtual ~PgDatabase();

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
    virtual void newNote(Note &) override;
    virtual void updateNote(const Note &) override;
    virtual void addTag(const int note_id, const int tag_id) override;
    virtual void removeTag(const int note_id, const int tag_id) override;
    virtual void deleteNote(const int note_id) override;
    virtual Note loadNote(const int note_id) override;

    virtual int newTag(const std::string &title) override;
    virtual std::vector<Tag> listTags() override;
    virtual void deleteTag(const int tag_id) override;

    virtual std::vector<Note>
    loadNotesFromNotebook(const int notebook_id) override;
    virtual std::vector<Note> loadNotesForTag(const int tag_id) override;

    virtual std::vector<Note> searchNotes(const std::string &term) override;

  private:
    pg_escaped_ptr escape(const std::string &str);
    // clear stmt_cache_
    void clearStatement();
    // execute statement saved in stmt_cache_
    pg_result_ptr executeStatement();

    // check if result gave the expected result code
    bool checkResultCode(PGresult *res, const int expect = PGRES_COMMAND_OK,
                         const std::string &msg = "Command failed");
    // check if result has expected number of rows and columns, if -1 is given
    // for either value it is ignored
    bool checkResultSize(PGresult *res, const int exp_rows,
                         const int exp_fields);

    // get the ID returned by a "RETURNING ..." statement (or any query
    // returning
    // single integer row/column field
    int getId(PGresult *res);

    // get string at pos row/field from result
    std::string getString(PGresult *res, const int row, const int field);
    // get integer at pos row/field from result
    int getInt(PGresult *res, const int row, const int field);
    // get timestamp at pos row/field from result, rely on PSQL format being
    // "YYYY-MM-DD hh:mm:ss"
    pt::ptime getTimestamp(PGresult *res, const int row, const int field);
};

} // db
} // notes
