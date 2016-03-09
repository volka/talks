project(DbSample)

cmake_minimum_required(VERSION 3.0)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Werror -Wextra -pedantic -Wconversion -Wno-unused-parameter")
#set(CMAKE_EXE_LINKER_FLAGS "-fPIC")
set(CMAKE_AUTORCC ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

add_definitions(-std=gnu++14)
find_package(PostgreSQL 9.4 REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Boost COMPONENTS date_time REQUIRED)
#find_package(Boost COMPONENTS date_time program_options REQUIRED)
#find_library(wtdbo NAMES libwtdbo.so HINTS ../../wt/wt-3.3.5/ /usr/lib64 /usr/lib)
#find_library(wtdbosqlite3 NAMES libwtdbosqlite3.so HINTS ../../wt/wt-3.3.5/ /usr/lib64 /usr/lib)
#find_library(wtdbopostgres NAMES libwtdbopostgres.so HINTS ../../wt/wt-3.3.5/ /usr/lib64 /usr/lib)
find_library(wtdbo NAMES libwtdbo.so)
find_library(wtdbosqlite3 NAMES libwtdbosqlite3.so)
find_library(wtdbopostgres NAMES libwtdbopostgres.so)
find_library(sqlpp11-sqlite3 NAMES libsqlpp11-connector-sqlite3.so HINTS . .. ../../sqlpp11-connector-sqlite3 )

find_package(Qt5 COMPONENTS Quick Sql Core Network)
#INCLUDE_DIRECTORIES("../../wt/wt-3.3.5/src/")
INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
# temp. relative include for SQLPP11
INCLUDE_DIRECTORIES("../../sqlpp11/include")
INCLUDE_DIRECTORIES("../../sqlpp11-connector-sqlite3/include")
INCLUDE_DIRECTORIES("../../date")
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
    include/db/pg_db.h
    src/db/pg_db.cpp

    # SQLite3 interface
    include/db/sqlite_db.h
    src/db/sqlite_db.cpp

    # QtSQL interface
    include/db/qt_db.h
    src/db/qt_db.cpp

    # Wt::Dbo
    include/db/wt_db.h
    src/db/wt_db.cpp

    # Sqlpp11
    include/db/sqlpp_db.h
    include/db/sqlpp_model.h
    src/db/sqlpp_db.cpp
)

set(CLIENT_INTERFACE include/client.h src/ui/client.cpp)
set(CLI_CLIENT include/ui/cli.h src/ui/cli.cpp)
set(QT_CLIENT include/ui/qt.h src/ui/qt.cpp)
set(QML_FILES resources/qtclient.qml)

qt5_add_resources(RESOURCES resources/notes.qrc)
set_source_files_properties(${RESOURCES} PROPERTIES GENERATED ON)

add_executable(notebook src/main.cpp ${SRC_LIST} ${CLIENT_INTERFACE}
                                     ${QT_CLIENT} ${CLI_CLIENT}
                                     ${RESOURCES} ${QML_FILES})

qt5_use_modules(notebook Quick Core)

target_link_libraries(notebook  ${PostgreSQL_LIBRARIES}
                                ${SQLITE3_LIBRARIES}
                                Qt5::Core
                                Qt5::Quick
                                Qt5::Sql
                                ${wtdbo}
								${wtdbopostgres}
								${wtdbosqlite3}
                                ${sqlpp11-sqlite3}
                                ${Boost_LIBRARIES})

add_executable(notebook_test src/test.cpp ${SRC_LIST})
target_link_libraries(notebook_test  ${PostgreSQL_LIBRARIES}
                                ${SQLITE3_LIBRARIES}
                                ${wtdbo}
								${wtdbopostgres}
								${wtdbosqlite3}
                                Qt5::Core
                                Qt5::Sql
                                ${sqlpp11-sqlite3}
                                ${Boost_LIBRARIES})

