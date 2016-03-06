#pragma once
#include "db_facade.h"

#include <QString>

namespace notes
{
namespace db
{

class QtDatabase : public NotebookDatabase
{

  public:
    QtDatabase(const QtDatabase &) = delete;

    // connection_info contains two parts: the DB type (QSQLITE, QPSQL)
    // followed by ':' and then DB specific connection string
    //
    // e.g. "QSQLITE::memory:", or "PSQL:dbname=postgres host=localhost"
    QtDatabase(const std::string &connection_info);
    virtual ~QtDatabase();

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
    ConnectionConfig config_;
};

} // ns db
} // ns notes
