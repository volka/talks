#pragma once

#include <string>

namespace notes {
namespace model {

class Tag {
  private:
    long long m_id;
    std::string m_title;

  public:
    inline long long id() { return m_id; }

    inline void id(long long id) { m_id = id; }

    inline const std::string &title() { return m_title; }

    inline void title(const std::string &title) { m_title = title; }
};

} // model
} // notes
