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
    time_t m_last_chang;
    time_t m_reminder;

  public:
    inline void id(const int id) { m_id = id; }
    inline int id() const { return m_id; }

    inline void id(const int id) { m_id = id; }

    inline const std::string &title() const { return m_title; }

    inline void title(const std::string &title) { m_title = title; }

    inline const std::string &content() const { return m_content; }

    inline void content(const std::string &content) { m_content = content; }
};

} // model
} // notes
