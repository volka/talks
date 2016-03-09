#pragma once

#include <string>
#include <boost/date_time.hpp>

namespace notes
{
namespace model
{

namespace pt = boost::posix_time;

// a note
class Note
{
  protected:
    int id_;
    std::string title_;
    std::string content_;
    int notebook_id_;
    pt::ptime last_change_;
    pt::ptime reminder_;

  public:
    Note()
        : id_(-1), title_(), content_(), notebook_id_(0), last_change_(),
          reminder_()
    {
    }

    Note(const Note &) = default;
    Note &operator=(const Note &) = default;
    Note(Note &&) = default;
    Note &operator=(Note &&) = default;

    // standard constructor filling all fields
    Note(const int id, const std::string &title, const std::string &content,
         const int notebook_id, const pt::ptime &last_change,
         const pt::ptime &reminder)
        : id_(id), title_(title), content_(content), notebook_id_(notebook_id),
          last_change_(last_change), reminder_(reminder)
    {
    }
    virtual ~Note() = default;

    // "new note inserter" missing the ID (ignored on insert anyways)
    Note(const std::string &title, const std::string &content,
         const int notebook_id, const pt::ptime &reminder)
        : id_(-1), title_(title), content_(content), notebook_id_(notebook_id),
          last_change_(), reminder_(reminder)
    {
    }

    inline int id() const { return id_; }
    inline void id(const int id) { id_ = id; }

    inline const std::string &title() const { return title_; }
    inline void title(const std::string &title) { title_ = title; }

    inline const std::string &content() const { return content_; }
    inline void content(const std::string &content) { content_ = content; }

    inline pt::ptime lastChanged() const { return last_change_; }
    inline void lastChanged(const pt::ptime time) { last_change_ = time; }

    inline pt::ptime reminder() const { return reminder_; }
    inline void reminder(const pt::ptime time) { reminder_ = time; }

    inline int notebook() const { return notebook_id_; }
    inline void notebook(const int notebook_id) { notebook_id_ = notebook_id; }
};

} // model
} // notes
