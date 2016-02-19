project(DbSample)

cmake_minimum_required(VERSION 3.0)
add_definitions(-std=gnu++14)
find_package(PostgreSQL 9.4 REQUIRED)
find_package(SQLite3 REQUIRED)

INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIR})
INCLUDE_DIRECTORIES(include)

set(SRC_LIST ${SRC_LIST}
    include/tag.h
    include/note.h
    include/notebook.h
    include/db_facade.h
    include/client.h
    include/pg/pq.h
    include/sqlite3/sqlite3.h
    include/ui/cli.h
    src/ui/cli.cpp
    include/ui/qt.h
	src/ui/qt.cpp
	src/db_facade.cpp
    src/main.cpp
)

add_executable(notebook_pg src/pg_main.cpp ${SRC_LIST})
target_link_libraries(notebook_pg ${PostgreSQL_LIBRARIES})

add_executable(notebook_sqlite src/sqlite_main.cpp ${SRC_LIST})
target_link_libraries(notebook_sqlite ${SQLITE3_LIBRARIES})
