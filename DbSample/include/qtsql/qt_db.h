#pragma once
#include "db_facade.h"

#include <QtSql/QSql>

namespace notes {
namespace db {

class QtDatabase : public NotebookDatabase {


    // NotebookDatabase interface
public:
    virtual void setupDb() override;
    virtual void fillDb() override;
    virtual std::vector<Notebook> listNotebooks() override;
    virtual int newNotebook(const std::__cxx11::string &title) override;
    virtual void renameNotebook(const int notebook_id, const std::__cxx11::string &new_title) override;
    virtual void deleteNotebook(const int id) override;
    virtual Notebook loadNotebook(const int notebook_id) override;
    virtual void newNote(Note &) override;
    virtual void updateNote(const Note &) override;
    virtual void addTag(const int note_id, const int tag_id) override;
    virtual void removeTag(const int note_id, const int tag_id) override;
    virtual void deleteNote(int id) override;
    virtual Note loadNote(int note_id) override;
    virtual int newTag(const std::__cxx11::string &title) override;
    virtual int findTag(const std::__cxx11::string &title) override;
    virtual void deleteTag(const int tag_id) override;
    virtual std::vector<Note> loadNotesFromNotebook(int notebook_id) override;
    virtual std::vector<Note> loadNotesForTag(int tag_id) override;
};

}
}
