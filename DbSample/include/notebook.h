#pragma once

#include <string>

namespace notes {
namespace model {

// a collection of notes
class Notebook {
  private:
    long long m_id;
    std::string m_title;

  public:
    inline long long id() const { return m_id; }

    inline void id(long long id) { m_id = id; }

    inline const std::string &title() const { return m_title; }

    inline void title(const std::string &title) { m_title = title; }
};

} // model
} // notes
