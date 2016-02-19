#include "db_facade.h"

#include "pg/pq.h"

std::unique_ptr<notes::db::NotebookDatabase>
notes::db::connect(const std::string &dbtype, const std::string &config) {
  if (dbtype == "sqlite3") {
  }
  if (dbtype == "postgres") {
  }
  if (dbtype == "qtsql-pg") {
  }
  if (dbtype == "qtsql-sqlite") {
  }
  if (dbtype == "wtdb-pg") {
  }
  if (dbtype == "wtdb-sqlite") {
  }
  if (dbtype == "sqlpp-pg") {
  }
  if (dbtype == "sqlpp-sqlite")

    switch (type) {
    case DbType::sqlite3:
      break;
    case DbType::libpq:
      return std::make_unique<>();
      break;
    case DbType::qtsql:
      break;
    case DbType::wt_db:
      break;
    case DbType::sqlpp11:
      break;
    }
}
