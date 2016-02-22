#include "db_facade.h"

#include "sqlite3/sqlite_db.h"
#include "pg/pg_db.h"

#include <memory>

std::unique_ptr<notes::db::NotebookDatabase>
notes::db::NotebookDatabase::create(const std::string& dbtype, const std::string& config)
{
    if (dbtype == Types::sqlite) {
        return std::make_unique<notes::db::Sqlite3Database>(config);
    }
    if (dbtype == Types::postgres) {
        return std::make_unique<notes::db::PgDatabase>(config);
    }
    if (dbtype == Types::qtsql) {

    }
    if (dbtype == Types::wt_db) {

    }
    if (dbtype == Types::sqlpp) {

    }

    throw DatabaseException("I don't know how to use a '" + dbtype + "' database...");
    return nullptr;
}

notes::db::NotebookDatabase::~NotebookDatabase() {}
