#pragma once

#include <exception>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "note.h"
#include "notebook.h"
#include "tag.h"

namespace notes {

namespace db {

using namespace notes::model;

// exceptions...
class DatabaseException : public std::domain_error {
    using std::domain_error::domain_error;
};

// abstract base class for Notebook Database interfaces, used for the GUI
class NotebookDatabase {
  public:
    // constants for database types
    struct Types {
        static constexpr auto sqlite = "sqlite";
        static constexpr auto postgres = "postgres";
        static constexpr auto qtsql = "qtsql";
        static constexpr auto wt_db = "wtdb";
        static constexpr auto sqlpp = "sqlpp";
    };

    // factory for DB connections, call with Db type
    static std::unique_ptr<NotebookDatabase> create(const std::string &type,
                                                    const std::string &config);

    virtual ~NotebookDatabase();

    // helper functions to initialize DB and seed with some sample data
    virtual void setupDb() = 0;
    virtual void fillDb() = 0;

    // create a notebook, return the generated ID
    virtual std::vector<Notebook> listNotebooks() = 0;
    virtual int newNotebook(const std::string &title) = 0;
    virtual void renameNotebook(const int notebook_id,
                                const std::string &new_title) = 0;
    virtual void deleteNotebook(const int id) = 0;
    virtual Notebook loadNotebook(const int notebook_id) = 0;

    // create a new note
    virtual void newNote(Note &) = 0;
    virtual void updateNote(const Note &) = 0;
    virtual void addTag(const int note_id, const int tag_id) = 0;
    virtual void removeTag(const int note_id, const int tag_id) = 0;
    virtual void deleteNote(int id) = 0;
    virtual Note loadNote(int note_id) = 0;

    virtual int newTag(const std::string &title) = 0;
    virtual int findTag(const std::string &title) = 0;
    virtual void deleteTag(const int tag_id) = 0;

    virtual std::vector<Note> loadNotesFromNotebook(int notebook_id) = 0;
    virtual std::vector<Note> loadNotesForTag(int tag_id) = 0;
};

} // db
} // notes
