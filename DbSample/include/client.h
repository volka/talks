#pragma once

#include "db_facade.h"

namespace notes {
namespace ui {

// base class for user interface clients of the Notes App
class Client {
public:
    Client(db::NotebookDatabase* db)
        : m_db(db)
    {}

    virtual ~Client();
    virtual void run() = 0;

private:
    db::NotebookDatabase* m_db;
};

} // ui
} // notes
