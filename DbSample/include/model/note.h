#pragma once

#include <string>
#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace notes {
namespace model {

namespace pt = boost::posix_time;

// a note
class Note {
  private:
    int m_id;
    std::string m_title;
    std::string m_content;
    int m_notebook_id;
    pt::ptime m_last_change;
    pt::ptime m_reminder;

  public:
    inline int id() const { return m_id; }
    inline void id(const int id) { m_id = id; }

    inline const std::string &title() const { return m_title; }
    inline void title(const std::string &title) { m_title = title; }

    inline const std::string &content() const { return m_content; }
    inline void content(const std::string &content) { m_content = content; }

    inline pt::ptime lastChanged() const { return m_last_change; }
    inline void lastChanged(const pt::ptime time) { m_last_change = time; }

    inline pt::ptime reminder() const { return m_reminder; }
    inline void reminder(const pt::ptime time) { m_reminder = time; }

    inline int notebook() const { return m_notebook_id; }
    inline void notebook(const int notebook_id) { m_notebook_id = notebook_id; }
};

} // model
} // notes
