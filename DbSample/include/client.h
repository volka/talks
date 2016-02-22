#pragma once

#include "db_facade.h"

namespace notes {
namespace ui {

// Generic UI exception
class UiException : public std::domain_error {
    using std::domain_error::domain_error;
};

// base class for user interface clients of the Notes App
class Client {
public:

    virtual ~Client() = 0;
    virtual void run() = 0;

    struct Types {
        static constexpr auto cli = "cli";
        static constexpr auto qt = "qt";
    };

    // factory method for clients
    static std::unique_ptr<Client> create(const std::string& type,
                                          std::shared_ptr<db::NotebookDatabase> &db);
};

} // ui
} // notes
