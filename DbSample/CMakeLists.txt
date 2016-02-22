project(DbSample)

cmake_minimum_required(VERSION 3.0)
add_definitions(-std=gnu++14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -Wconversion")
find_package(PostgreSQL 9.4 REQUIRED)
find_package(SQLite3 REQUIRED)

INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIR})
INCLUDE_DIRECTORIES(include)

set(SRC_LIST ${SRC_LIST}
    # model
    include/tag.h
    include/note.h
    include/notebook.h

    # DB interface code
    include/db_facade.h
    src/db_facade.cpp

    # PostgreSQL interface
    include/pg/pg_db.h
    src/db/pg_db.cpp

    # SQLite3 interface
    include/sqlite3/sqlite_db.h
    src/db/sqlite_db.cpp

    # UI code
    include/client.h
    src/ui/client.cpp

    # CLI
    include/ui/cli.h
    src/ui/cli.cpp

    # QT
    include/ui/qt.h
    src/ui/qt.cpp
)

add_executable(notebook src/main.cpp ${SRC_LIST})
target_link_libraries(notebook ${PostgreSQL_LIBRARIES})
target_link_libraries(notebook ${SQLITE3_LIBRARIES})

#add_executable(notebook_pg src/pg_main.cpp ${SRC_LIST})
#target_link_libraries(notebook_pg ${PostgreSQL_LIBRARIES})

#add_executable(notebook_sqlite src/sqlite_main.cpp ${SRC_LIST})
#target_link_libraries(notebook_sqlite ${SQLITE3_LIBRARIES})
