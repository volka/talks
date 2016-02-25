#pragma once

#include <string>

namespace notes {
namespace model {

// a note
class Note {
  private:
    int m_id;
    std::string m_title;
    std::string m_content;
    int m_notebook_id;
    time_t m_last_change;
    time_t m_reminder;

  public:
    inline int id() const { return m_id; }
    inline void id(const int id) { m_id = id; }

    inline const std::string &title() const { return m_title; }
    inline void title(const std::string &title) { m_title = title; }

    inline const std::string &content() const { return m_content; }
    inline void content(const std::string &content) { m_content = content; }

    inline time_t lastChanged() const { return m_last_change; }
    inline void lastChanged(const time_t time) { m_last_change = time; }

    inline time_t reminder() const { return m_reminder; }
    inline void reminder(const time_t time) { m_reminder = time; }

    inline int notebook() const { return m_notebook_id; }
    inline void notebook(const int notebook_id) { m_notebook_id = notebook_id; }
};

} // model
} // notes
