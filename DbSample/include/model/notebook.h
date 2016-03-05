#pragma once

#include <string>

namespace notes
{
namespace model
{

// a collection of notes
class Notebook
{
  private:
    long long id_;
    std::string title_;

  public:
    inline long long id() const { return id_; }

    inline void id(long long id) { id_ = id; }

    inline const std::string &title() const { return title_; }

    inline void title(const std::string &title) { title_ = title; }
};

} // model
} // notes
