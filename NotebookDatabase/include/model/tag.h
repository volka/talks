#pragma once

#include <string>

namespace notes
{
namespace model
{

using bigint_t = long long;

class Tag
{
  protected:
    bigint_t id_;
    std::string title_;

  public:
    Tag() : id_(-1), title_() {}

    Tag(const Tag &) = default;
    Tag &operator=(const Tag &) = default;
    Tag(Tag &&) = default;
    Tag &operator=(Tag &&) = default;

    Tag(const std::string &title) : id_(-1), title_(title) {}

    Tag(const bigint_t id, const std::string &title) : id_(id), title_(title) {}

    virtual ~Tag() = default;

    inline bigint_t id() const { return id_; }

    inline void id(bigint_t id) { id_ = id; }

    inline const std::string &title() const { return title_; }

    inline void title(const std::string &title) { title_ = title; }
};

} // model
} // notes
