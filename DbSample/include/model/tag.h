#pragma once

#include <string>

namespace notes
{
namespace model
{

class Tag
{
  protected:
    int id_;
    std::string title_;

  public:
    Tag() : id_(-1), title_() {}

    Tag(const Tag &) = default;
    Tag &operator=(const Tag &) = default;
    Tag(Tag &&) = default;
    Tag &operator=(Tag &&) = default;

    Tag(const std::string &title) : id_(-1), title_(title) {}

    Tag(const int id, const std::string &title) : id_(id), title_(title) {}

    inline int id() { return id_; }

    inline void id(int id) { id_ = id; }

    inline const std::string &title() { return title_; }

    inline void title(const std::string &title) { title_ = title; }
};

} // model
} // notes
