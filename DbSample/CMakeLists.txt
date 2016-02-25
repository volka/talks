project(DbSample)

cmake_minimum_required(VERSION 3.0)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -Wconversion")

set(CMAKE_AUTORCC ON)

add_definitions(-std=gnu++14)
find_package(PostgreSQL 9.4 REQUIRED)
find_package(SQLite3 REQUIRED)

# Find the QtWidgets library
#find_package(Qt5Core)
#find_package(Qt5Quick2)
#find_package(Qt5Widgets)

find_package(Qt5 COMPONENTS Quick Core Network)
INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIR})
INCLUDE_DIRECTORIES(include)

set(SRC_LIST
    # model
    include/model/tag.h
    include/model/note.h
    include/model/notebook.h

    # DB interface code
    include/db_facade.h
    src/db_facade.cpp

    # PostgreSQL interface
    include/pg/pg_db.h
    src/db/pg_db.cpp

    # SQLite3 interface
    include/sqlite3/sqlite_db.h
    src/db/sqlite_db.cpp

    # QtSQL interface
    include/qtsql/qt_db.h
    src/db/qt_db.cpp

    # UI code
    include/client.h
    src/ui/client.cpp
)

set(CLI_CLIENT include/ui/cli.h src/ui/cli.cpp)
set(QT_CLIENT include/ui/qt.h src/ui/qt.cpp)
set(QML_FILES resources/qtclient.qml)

qt5_add_resources(RESOURCES resources/notes.qrc)
set_source_files_properties(${RESOURCES} PROPERTIES GENERATED ON)

add_executable(notebook src/main.cpp ${SRC_LIST} ${QT_CLIENT} ${CLI_CLIENT}
                                     ${RESOURCES} ${QML_FILES})

qt5_use_modules(notebook Quick Core)

target_link_libraries(notebook ${PostgreSQL_LIBRARIES})
target_link_libraries(notebook ${SQLITE3_LIBRARIES})
target_link_libraries(notebook Qt5::Core)
target_link_libraries(notebook Qt5::Quick)

#add_executable(notebook_pg src/pg_main.cpp ${SRC_LIST})
#target_link_libraries(notebook_pg ${PostgreSQL_LIBRARIES})

#add_executable(notebook_sqlite src/sqlite_main.cpp ${SRC_LIST})
#target_link_libraries(notebook_sqlite ${SQLITE3_LIBRARIES})
