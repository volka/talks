#include "db_facade.h"

#include <Wt/Dbo/Dbo>

namespace notes
{
namespace db
{
namespace wt
{
class Tag;
class Notebook;
class Note;
}
}
}
/*
namespace Wt
{
namespace Dbo
{
template <> struct dbo_traits<notes::db::wt::Tag> : public dbo_default_traits {
    typedef long long IdType;
    static IdType invalidId() { return -1; }
    static const char *surrogateIdField() { return 0; }
};
template <>
struct dbo_traits<notes::db::wt::Notebook> : public dbo_default_traits {
    typedef long long IdType;
    static IdType invalidId() { return -1; }
    static const char *surrogateIdField() { return 0; }
};
template <> struct dbo_traits<notes::db::wt::Note> : public dbo_default_traits {
    typedef long long IdType;
    static IdType invalidId() { return -1; }
    static const char *surrogateIdField() { return 0; }
};
} // ns Dbo
} // ns Wt
*/
namespace notes
{
namespace db
{

// Wt::Dbo object namespace
namespace wt
{
namespace dbo = Wt::Dbo;
using bigint_t = long long;

class Note;

// table tags
class Tag : public model::Tag
{
  public:
    Tag() = default;
    Tag(const model::Tag &tag) : model::Tag(tag) {}

    dbo::collection<dbo::ptr<wt::Note>> notes;

    template <class Action> void persist(Action &a)
    {
        // dbo::id(a, id_, "id");
        dbo::field(a, title_, "title");
        dbo::hasMany(a, notes, dbo::ManyToMany, "tags_nm");
    }
};

// table notebooks
class Notebook : public model::Notebook
{
  public:
    Notebook() = default;
    Notebook(const model::Notebook &notebook) : model::Notebook(notebook) {}

    dbo::collection<dbo::ptr<wt::Note>> notes;

    template <class Action> void persist(Action &a)
    {
        // dbo::id(a, id_, "id");
        dbo::field(a, title_, "title");
        dbo::hasMany(a, notes, dbo::ManyToOne, "notebook");
    }
};

// table notes
class Note : public model::Note
{
  public:
    Note() = default;
    Note(const model::Note &note) : model::Note(note) {}

    dbo::collection<dbo::ptr<wt::Tag>> tags;
    dbo::ptr<wt::Notebook> notebook;

    template <class Action> void persist(Action &a)
    {
        // field name, name in DB
        // dbo::id(a, id_, "id");
        dbo::field(a, title_, "title");
        dbo::field(a, content_, "content");
        dbo::belongsTo(a, notebook, "notebook", dbo::OnDeleteCascade);
        dbo::field(a, last_change_, "last_change");
        dbo::field(a, reminder_, "reminder");
        dbo::hasMany(a, tags, dbo::ManyToMany, "tags_nm");
    }
};

} // ns wt

class WtDatabase : public NotebookDatabase
{

    // NotebookDatabase interface
  public:
    WtDatabase(const std::string &conn_info);
    virtual ~WtDatabase();

    virtual void setupDb() override;

    virtual std::vector<Notebook> listNotebooks() override;
    virtual bigint_t newNotebook(const std::string &title) override;
    virtual void renameNotebook(const bigint_t notebook_id,
                                const std::string &new_title) override;
    virtual void deleteNotebook(const bigint_t id) override;
    virtual Notebook loadNotebook(const bigint_t notebook_id) override;

    virtual void newNote(Note &) override;
    virtual void updateNote(const Note &) override;
    virtual void addTag(const bigint_t note_id, const bigint_t tag_id) override;
    virtual void removeTag(const bigint_t note_id,
                           const bigint_t tag_id) override;
    virtual void deleteNote(const bigint_t note_id) override;
    virtual Note loadNote(const bigint_t note_id) override;

    virtual bigint_t newTag(const std::string &title) override;
    virtual std::vector<Tag> listTags() override;
    virtual void deleteTag(const bigint_t tag_id) override;

    virtual std::vector<Note>
    loadNotesFromNotebook(const bigint_t notebook_id) override;
    virtual std::vector<Note> loadNotesForTag(const bigint_t tag_id) override;
    virtual std::vector<Note> searchNotes(const std::string &term) override;

  private:
    NotebookDatabase::ConnectionConfig config_;
    Wt::Dbo::Session session_;
    std::unique_ptr<Wt::Dbo::SqlConnection> conn_;
};

} // ns db
} // ns notes
