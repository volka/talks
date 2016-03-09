#pragma once

#include <string>

namespace notes
{
namespace model
{

using bigint_t = long long;

// a collection of notes
class Notebook
{
  protected:
    bigint_t id_;
    std::string title_;

  public:
    Notebook() : id_(-1), title_() {}

    Notebook(const Notebook &) = default;
    Notebook &operator=(const Notebook &) = default;
    Notebook(Notebook &&) = default;
    Notebook &operator=(Notebook &&) = default;

    Notebook(const std::string &title) : id_(-1), title_(title) {}

    Notebook(const bigint_t id, const std::string &title)
        : id_(id), title_(title)
    {
    }

    virtual ~Notebook() = default;

    inline bigint_t id() const { return id_; }

    inline void id(bigint_t id) { id_ = id; }

    inline const std::string &title() const { return title_; }

    inline void title(const std::string &title) { title_ = title; }
};

} // model
} // notes
