#include "db_facade.h"

#include "db/sqlite_db.h"
#include "db/pg_db.h"
#include "db/qt_db.h"
#include "db/wt_db.h"
#include "db/sqlpp_db.h"

#include <string>
#include <memory>

namespace notes
{
namespace db
{

namespace
{

// this parses the PSQL specific fields
void parsePgConfig(const std::string &conn_info,
                   NotebookDatabase::ConnectionConfig &config)
{
    using namespace std;
    // parse dbname, host, username, password - separated by spaces
    size_t space_pos = 0;
    string option, value;
    while (space_pos != string::npos) {

        option.clear();
        value.clear();

        auto eq_pos = conn_info.find("=", space_pos);
        if (eq_pos == string::npos)
            return;
        option = conn_info.substr(space_pos, eq_pos - space_pos);

        // end of value, also next option ...
        space_pos = conn_info.find(" ", eq_pos);
        if (space_pos == string::npos)
            space_pos = conn_info.size();
        ++eq_pos;

        value = conn_info.substr(eq_pos, space_pos - eq_pos);

        if (space_pos != string::npos)
            ++space_pos;

        if (option == "dbname")
            config.dbname = value;
        else if (option == "host")
            config.host = value;
        else if (option == "port")
            config.port = value;
        else if (option == "username")
            config.username = value;
        else if (option == "password")
            config.password = value;
        else
            throw DatabaseException("Invalid option in connection_info: " +
                                    option);
    }
    return;
}
} // ns anon

// parse the connection_info for QSQL to separate the DRIVER and Parameters
NotebookDatabase::ConnectionConfig
parseConnectionInfo(const std::string &conn_info)
{
    NotebookDatabase::ConnectionConfig config;

    size_t split = conn_info.find(":");

    if (split != std::string::npos) {
        config.driver = conn_info.substr(0, split);
        ++split;
        config.conn_str = conn_info.substr(split, conn_info.size() - split);

        if (config.driver == "QSQLITE" || config.driver == "SQLITE") {
            config.dbname = conn_info.substr(split, conn_info.size() - split);
            if (config.dbname.empty())
                throw DatabaseException(
                    "Invalid DB name for Sqlite connection");
        } else if (config.driver == "QPSQL" || config.driver == "PG") {
            config.port = "5432"; // set default
            config.host = "/var/run/postgresql/";
            parsePgConfig(conn_info.substr(split, conn_info.size() - split),
                          config);
        } else {
            throw DatabaseException("Unsupported driver for QtSql/Wt: " +
                                    config.driver);
        }
    } else
        throw DatabaseException(
            "Invalid connection string, could not find ':'");

    return config;
}

std::unique_ptr<NotebookDatabase>
NotebookDatabase::create(const std::string &dbtype, const std::string &config)
{
    if (dbtype == Types::sqlite) {
        return std::make_unique<Sqlite3Database>(config);
    }
    if (dbtype == Types::postgres) {
        return std::make_unique<PgDatabase>(config);
    }
    if (dbtype == Types::qtsql) {
        return std::make_unique<QtDatabase>(config);
    }
    if (dbtype == Types::wt_db) {
        return std::make_unique<WtDatabase>(config);
    }
    if (dbtype == Types::sqlpp) {
        return std::make_unique<SqlppDatabase>(config);
    }

    throw DatabaseException("I don't know how to use a '" + dbtype +
                            "' database...");
    return nullptr;
}

NotebookDatabase::~NotebookDatabase() {}

// create some dummy test data ...
void NotebookDatabase::fillDb()
{
    int nb_id = newNotebook("Privat");
    Notebook privat = loadNotebook(nb_id);

    Note folien("Vortrag", "Folien fertigschreiben", privat.id(),
                pt::ptime(pt::time_from_string("2016-03-10 18:59:59")));
    Note code("Beispielcode", "Code schreiben und testen", privat.id(),
              pt::ptime(pt::time_from_string("2016-03-10 18:59:59")));

    newNote(folien);
    newNote(code);

    int cpp_tag_id = newTag("C++");
    addTag(folien.id(), cpp_tag_id);
    addTag(code.id(), cpp_tag_id);

    int nb_work = newNotebook("Einkaufslisten");
    Notebook work = loadNotebook(nb_work);
    Note essen("Essen", "* Eier\n*Milch\n*Mehl\n* Nutella", work.id(),
               pt::ptime(pt::time_from_string("2016-03-11 12:00:00")));
    newNote(essen);
}

} // ns db
} // ns notes
