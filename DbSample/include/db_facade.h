#pragma once

#include <exception>
#include <string>
#include <map>
#include <vector>

namespace notes {

// forward decl.
namespace model {
    class Note;
    class Notebook;
    class Tag;
}

namespace db {

using namespace notes::model;

// exceptions...
class DatabaseException : public std::exception {};

// DB-specific configuration
using DbConfig = std::map<std::string, std::string>;

enum class DbType {
    sqlite3,
    libpq,
    qtsql,
    wt_db,
    sqlpp11
};

// abstract base class for Notebook Database interfaces, used for the GUI
class NotebookDatabase {
public:
    NotebookDatabase() = delete;
    virtual ~NotebookDatabase();

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

// factory for DB connections, call with Db type
std::unique_ptr<NotebookDatabase> connect(DbType type, DbConfig& config);

} // db
} // notes
