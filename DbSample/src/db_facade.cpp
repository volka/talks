#include "db_facade.h"

std::unique_ptr<notes::db::NotebookDatabase> notes::db::connect(notes::db::DbType type, notes::db::DbConfig &config)
{
    switch (type) {
    case DbType::sqlite3:
        break;
    case DbType::libpq:
        break;
    case DbType::qtsql:
        break;
    case DbType::wt_db:
        break;
    case DbType::sqlpp11:
        break;
    }

}
