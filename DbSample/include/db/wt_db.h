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

// TODO
// table notes
class Note
{
  public:
    int id;
    std::string title;
    std::string content;
    int notebook_id;
    pt::ptime last_change;
    pt::ptime reminder;

    template <class Action> void persist(Action &a)
    {
        dbo::field(a, id, "id");
        dbo::field(a, title, "title");
        dbo::field(a, content, "content");
        dbo::field(a, notebook_id, "notebook");
        dbo::field(a, last_change, "last_change");
        dbo::field(a, reminder, "reminder");
    }
};

// table tags , tags_nm
class Tag
{
  public:
    int id;
    std::string title;
};

// table notebooks
class Notebook
{
  public:
    int id;
    std::string title;
};

} // ns wt

class WtDatabase : public NotebookDatabase
{

    // NotebookDatabase interface
  public:
    WtDatabase(const std::string &conn_info);
    virtual ~WtDatabase();

    virtual void setupDb() override;
    virtual void fillDb() override;
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
    virtual void deleteTag(const int tag_id) override;
    virtual std::vector<Note> loadNotesFromNotebook(int notebook_id) override;
    virtual std::vector<Note> loadNotesForTag(int tag_id) override;

  private:
    NotebookDatabase::ConnectionConfig config_;
};

} // ns db
} // ns notes
