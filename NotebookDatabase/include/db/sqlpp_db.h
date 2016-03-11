#pragma once

#include "db_facade.h"
#include "sqlpp_model.h"

#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/sqlite3/connection.h>

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
    virtual bigint_t newNotebook(const std::string &title) override;
    virtual void renameNotebook(const bigint_t notebook_id,
                                const std::string &new_title) override;
    virtual void deleteNotebook(const bigint_t id) override;
    virtual Notebook loadNotebook(const bigint_t notebook_id) override;

    virtual void newNote(Note &note) override;
    virtual void updateNote(const Note &note) override;
    virtual void addTag(const bigint_t note_id, const bigint_t tag_id) override;
    virtual void removeTag(const bigint_t note_id,
                           const bigint_t tag_id) override;
    virtual void deleteNote(const bigint_t id) override;
    virtual Note loadNote(const bigint_t note_id) override;

    virtual bigint_t newTag(const std::string &title) override;
    virtual std::vector<Tag> listTags() override;
    virtual void deleteTag(const bigint_t tag_id) override;

    virtual std::vector<Note>
    loadNotesFromNotebook(const bigint_t notebook_id) override;
    virtual std::vector<Note> loadNotesForTag(const bigint_t tag_id) override;
    virtual std::vector<Note> searchNotes(const std::string &term) override;

  private:
    // sqlpp model tables
    sqlpp_model::Notebooks notebooks_;
    sqlpp_model::Notes notes_;
    sqlpp_model::Tags tags_;
    sqlpp_model::Tags_NM tags_nm_;

    sqlpp::sqlite3::connection &conn();
    std::string connection_info_;
    std::unique_ptr<sqlpp::sqlite3::connection> connection_;
};

} // ns db
} // ns notes
