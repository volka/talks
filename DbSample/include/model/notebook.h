#pragma once

#include <string>

namespace notes
{
namespace model
{

// a collection of notes
class Notebook
{
  protected:
    int id_;
    std::string title_;

  public:
    Notebook() : id_(-1), title_() {}

    Notebook(const Notebook &) = default;
    Notebook &operator=(const Notebook &) = default;
    Notebook(Notebook &&) = default;
    Notebook &operator=(Notebook &&) = default;

    Notebook(const std::string &title) : id_(-1), title_(title) {}

    Notebook(const int id, const std::string &title) : id_(id), title_(title) {}

    virtual ~Notebook() = default;

    inline int id() const { return id_; }

    inline void id(int id) { id_ = id; }

    inline const std::string &title() const { return title_; }

    inline void title(const std::string &title) { title_ = title; }
};

} // model
} // notes
