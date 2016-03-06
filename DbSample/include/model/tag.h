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
    Tag(const std::string &title) : id_(-1), title_(title) {}

    Tag(const int id, const std::string &title) : id_(id), title_(title) {}

    inline long long id() { return id_; }

    inline void id(long long id) { id_ = id; }

    inline const std::string &title() { return title_; }

    inline void title(const std::string &title) { title_ = title; }
};

} // model
} // notes
