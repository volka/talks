#pragma once

#include <string>

namespace notes
{
namespace model
{

class Tag
{
  private:
    long long id_;
    std::string title_;

  public:
    inline long long id() { return id_; }

    inline void id(long long id) { id_ = id; }

    inline const std::string &title() { return title_; }

    inline void title(const std::string &title) { title_ = title; }
};

} // model
} // notes
