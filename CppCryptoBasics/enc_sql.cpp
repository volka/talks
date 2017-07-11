#include <sqlcipher/sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

int exec(sqlite3* db, const std::string& sql, const std::vector<int> expected_res = {SQLITE_OK})
{
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.data(), sql.size(), &stmt, 0);
    int res = sqlite3_step(stmt);
    if (std::find(expected_res.begin(), expected_res.end(), res) == expected_res.end())
    {
      std::cout << "command failed: " << sql << " (got " << res << ")\n";
    }
    sqlite3_finalize(stmt);
    return res;
}

int main()
{
    std::string password{"foobar"};
    sqlite3* db;

    int res = sqlite3_open("test.db", &db);
    if (res != SQLITE_OK)
      return -1;

    res = sqlite3_key(db, password.data(), password.size());
    if (res != SQLITE_OK)
      std::cout << "key failed\n";

    exec(db, "drop table test", {SQLITE_DONE, SQLITE_MISUSE});
    exec(db, "create table test (name text not null)", {SQLITE_DONE});
    exec(db, "insert into test values ('hello world')", {SQLITE_DONE});
    sqlite3_close(db);
}
