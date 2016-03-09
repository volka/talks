#include "db_facade.h"

#include <Wt/Dbo/Dbo>

namespace notes
{
namespace db
{

// Wt::Dbo object namespace
namespace wt
{
namespace dbo = Wt::Dbo;

class Note;

// table tags
class Tag : public model::Tag
{
  public:
    Tag() = default;
    Tag(const model::Tag &tag) : model::Tag(tag) {}

    dbo::collection<dbo::ptr<Note>> notes;

    template <class Action> void persist(Action &a)
    {
        //dbo::field(a, id_, "id");
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

    dbo::collection<dbo::ptr<Note>> notes;

    template <class Action> void persist(Action &a)
    {
        //dbo::field(a, id_, "id");
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

    dbo::collection<dbo::ptr<Tag>> tags;
    dbo::ptr<Notebook> notebook;

    template <class Action> void persist(Action &a)
    {
        // field name, name in DB
        //dbo::field(a, id_, "id");
        dbo::field(a, title_, "title");
        dbo::field(a, content_, "content");
        dbo::belongsTo(a, notebook, "notebook");
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
    virtual int newNotebook(const std::string &title) override;
    virtual void renameNotebook(const int notebook_id,
                                const std::string &new_title) override;
    virtual void deleteNotebook(const int id) override;
    virtual Notebook loadNotebook(const int notebook_id) override;

    virtual void newNote(Note &) override;
    virtual void updateNote(const Note &) override;
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
    NotebookDatabase::ConnectionConfig config_;
    Wt::Dbo::Session session_;
    std::unique_ptr<Wt::Dbo::SqlConnection> conn_;
};

} // ns db
} // ns notes
