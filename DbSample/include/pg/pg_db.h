#pragma once

#include "db_facade.h"
#include <libpq-fe.h>


namespace notes {
namespace db {

// wrap plain C pointers in RAII types...

// PGconn is finalized by PQfinish
using pg_conn_ptr = std::unique_ptr<PGconn, std::function<void(PGconn*)>>;
// a PGresult obtained e.g. through PQexec, needs to be freed using PQclear
using pg_result_ptr = std::unique_ptr<PGresult, std::function<void(PGresult*)>>;
// an esacped string as produced by PQescapeLiteral, free with PQfreemem
using pg_escaped_ptr = std::unique_ptr<char, std::function<void(void*)>>;


class PgDatabase : public NotebookDatabase {
private:
    std::string connection_info_;
    pg_conn_ptr connection_;

public:

    PgDatabase(const PgDatabase&) = delete;
    PgDatabase(const std::string& connection_info);
    virtual ~PgDatabase();

    // helper functions to initialize DB and seed with some sample data
    virtual void setupDb();
    virtual void fillDb();

    // create a notebook, return the generated ID
    virtual int newNotebook(const std::string& title);
    virtual void renameNotebook(const int notebook_id, const std::string& new_title);
    virtual void deleteNotebook(const int id);
    virtual std::unique_ptr<Notebook> loadNotebook(const int notebook_id);

    // create a new note
    virtual void newNote(Note&);
    virtual void updateNote(const Note&);
    virtual void addTag(const int note_id, const int tag_id);
    virtual void removeTag(const int note_id, const int tag_id);
    virtual void deleteNote(int id);
    virtual std::unique_ptr<Note> loadNote(int note_id);


    virtual int newTag(const std::string& title);
    virtual int findTag(const std::string& title);
    virtual void deleteTag(const int tag_id);

    virtual std::vector<Note> loadNotesFromNotebook(int notebook_id);
    virtual std::vector<Note> loadNotesForTag(int tag_id);
};

} // db
} // notes
