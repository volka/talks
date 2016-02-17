project(DbSample)

cmake_minimum_required(VERSION 3.2)
add_definitions(-std=c++1y)
find_package(PostgreSQL 9.5 REQUIRED)
find_package(SQLite3 REQUIRED)

INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIR})
INCLUDE_DIRECTORIES(include)

set(SRC_LIST ${SRC_LIST}
    include/tag.h
    include/note.h
    include/notebook.h
	include/db_facade.h
    include/cli.h
    src/ui/cli.cpp
	include/qt.h
	src/ui/qt.cpp
	src/db_facade.cpp
)

add_executable(notebook_pg src/pg_main.cpp ${SRC_LIST})
target_link_libraries(notebook_pg ${PostgreSQL_LIBRARIES})

add_executable(notebook_sqlite src/sqlite_main.cpp ${SRC_LIST})
target_link_libraries(notebook_sqlite ${SQLITE3_LIBRARIES})
