#include <iostream>
#include <sstream>
#include <memory>
#include <ctime>

#include <libpq-fe.h>

#include "model/note.h"
#include "model/tag.h"
#include "model/notebook.h"

using std::cout;
using std::endl;

// constants
const char *DEFAULT_TIME = "0000-00-00T00:00:00";

// wrap plain C pointers in RAII types...
using pg_conn_ptr = std::unique_ptr<PGconn, std::function<void(PGconn *)>>;
using pg_result_ptr =
    std::unique_ptr<PGresult, std::function<void(PGresult *)>>;
using pg_escaped_ptr = std::unique_ptr<char, std::function<void(void *)>>;

// helper for result validation
bool checkResult(PGconn *conn, PGresult *res, int expected = PGRES_COMMAND_OK,
                 const std::string &msg = "Command failed")
{
    if (res == nullptr || PQresultStatus(res) != expected) {
        cout << msg << ": " << PQerrorMessage(conn);
        return false;
    }
    return true;
}

int get_id(PGresult *res)
{
    int fields = PQnfields(res);
    if (fields != 1)
        throw std::runtime_error(
            "RETURNING id query did not return one field as expected");

    int id = -1;
    for (int i = 0; i < PQntuples(res); ++i) {
        id = atoi(PQgetvalue(res, i, 0));
        cout << "Field " << PQfname(res, 0) << ": " << PQgetvalue(res, i, 0);
    }
    return id;
}

// create our tables and fill with sample values ...
void setup(PGconn *conn)
{
    auto result = pg_result_ptr(
        PQexec(conn, "DROP TABLE IF EXISTS tags_nm, notes, tags, notebooks;"),
        PQclear);

    if (!checkResult(conn, result.get()))
        throw std::runtime_error("dropping pre-existing tables failed");

    result = pg_result_ptr(PQexec(conn, "CREATE TABLE notebooks ("
                                        "id		serial primary key,"
                                        "title	varchar(255)"
                                        ")"),
                           PQclear);
    if (!checkResult(conn, result.get()))
        throw std::runtime_error("creating table notebooks failed");

    result = pg_result_ptr(PQexec(conn, "CREATE TABLE tags ("
                                        "id	 	serial primary key,"
                                        "title	varchar(255)"
                                        ")"),
                           PQclear);

    if (!checkResult(conn, result.get()))
        throw std::runtime_error("creating table tags failed");

    result = pg_result_ptr(
        PQexec(conn, "CREATE TABLE notes ("
                     "id      	serial primary key,"
                     "title   	varchar(255),"
                     "content		text,"
                     "notebook 	int references notebooks(id),"
                     "last_change timestamp DEFAULT CURRENT_TIMESTAMP,"
                     "reminder	timestamp"
                     ")"),
        PQclear);

    if (!checkResult(conn, result.get()))
        throw std::runtime_error("creating table notes failed");

    result = pg_result_ptr(
        PQexec(conn, "CREATE TABLE tags_nm ("
                     "tag_id		serial references tags(id),"
                     "note_id	serial references notes(id)"
                     ")"),
        PQclear);

    if (!checkResult(conn, result.get()))
        throw std::runtime_error("creating table tags_nm failed");
}

void free_char(char *test) { PQfreemem((void *)test); }

int insert_note(PGconn *conn, const std::string &title,
                const std::string content, int notebook, time_t reminder)
{
    auto title_str = std::shared_ptr<char>(
        PQescapeLiteral(conn, title.c_str(), title.size()), PQfreemem);
    auto content_str = std::shared_ptr<char>(
        PQescapeLiteral(conn, content.c_str(), content.size()), PQfreemem);

    // formatting the reminder correctly, ignoring timezones
    std::string reminder_str = DEFAULT_TIME;
    std::tm tmp;
    localtime_r(&reminder, &tmp);

    if (strftime(const_cast<char *>(reminder_str.data()), sizeof(reminder_str),
                 "%F %T", &tmp) == 0) {
        reminder_str = DEFAULT_TIME;
    }

    std::stringstream stmt;
    stmt << "INSERT INTO notes(title, content, notebook, reminder) VALUES("
         << title_str.get() << ", " << content_str.get() << ", " << notebook
         << ", '" << reminder_str << "') RETURNING id";
    auto result = pg_result_ptr(PQexec(conn, stmt.str().c_str()), PQclear);
    if (!checkResult(conn, result.get(), PGRES_TUPLES_OK))
        throw std::runtime_error("inserting note " + title + " failed");

    return get_id(result.get());
}

/*
int insert_node(Note& note)
{
    insert_note(note.)
}
*/

int insert_tag(PGconn *conn, const std::string &title)
{
    auto title_str = std::shared_ptr<char>(
        PQescapeLiteral(conn, title.c_str(), title.size()), PQfreemem);
    return 1;
}

int insert_notebook(PGconn *conn, const std::string &title)
{
    auto title_str = pg_escaped_ptr(
        PQescapeLiteral(conn, title.c_str(), title.size()), PQfreemem);

    std::stringstream stmt;
    stmt << "INSERT INTO notebooks(title) VALUES(" << title_str.get()
         << ") RETURNING id";

    auto result = pg_result_ptr(PQexec(conn, stmt.str().c_str()), PQclear);
    if (!checkResult(conn, result.get(), PGRES_TUPLES_OK))
        throw std::runtime_error("inserting notebook " + title + " failed");

    return get_id(result.get());
}

int main(int argc, char **argv)
{
    const char *conninfo = "host=/tmp dbname=postgres";
    if (argc > 1) {
        conninfo = argv[1];
    }
    cout << ">> connecting to " << conninfo << endl;
    auto conn = pg_conn_ptr(PQconnectdb(conninfo), PQfinish);
    if (PQstatus(conn.get()) != 0) {
        cout << "connecting to DB failed: " << PQerrorMessage(conn.get())
             << endl;
        exit(-1);
    }

    try {

        setup(conn.get());
        int notebook_id = insert_notebook(conn.get(), "Meine Notizen");
        insert_note(conn.get(), "Foo", "Dies ist eine Test Notiz", notebook_id,
                    time(0));

    } catch (std::runtime_error &ex) {
        cout << "error: " << ex.what() << endl;
        exit(-1);
    }
    return 0;
}
