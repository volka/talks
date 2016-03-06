#pragma once

#include "db_facade.h"

#include <sqlpp11/sqlpp11.h>

namespace notes
{
namespace db
{

class SqlppDatabase : public NotebookDatabase
{

  public:
    SqlppDatabase(const SqlppDatabase &) = delete;

    // only SQLite is supported for SQLPP11 at the moment,
    // just pass the DB path in the connection_info
    SqlppDatabase(const std::string &connection_info);
    virtual ~SqlppDatabase();

    // NotebookDatabase interface
    virtual void setupDb() override;
    virtual std::vector<Notebook> listNotebooks() override;
    virtual int newNotebook(const std::string &title) override;
    virtual void renameNotebook(const int notebook_id,
                                const std::string &new_title) override;
    virtual void deleteNotebook(const int id) override;
    virtual Notebook loadNotebook(const int notebook_id) override;

    virtual void newNote(Note &) override;
    virtual void updateNote(const Note &) override;
    virtual void addTag(const int note_id, const int tag_id) override;
    virtual void removeTag(const int note_id, const int tag_id) override;
    virtual void deleteNote(const int id) override;
    virtual Note loadNote(const int note_id) override;

    virtual int newTag(const std::string &title) override;
    virtual std::vector<Tag> listTags() override;
    virtual void deleteTag(const int tag_id) override;

    virtual std::vector<Note>
    loadNotesFromNotebook(const int notebook_id) override;
    virtual std::vector<Note> loadNotesForTag(const int tag_id) override;
    virtual std::vector<Note> searchNotes(const std::string &term) override;

  private:
    std::string connection_info_;
};

} // ns db
} // ns notes
