# talks
Slides and demo code for talks ...

## Serialization in C++ (german)

This talk introduces basic concepts of data serialization and presents several common C++ serialization frameworks. I demonstrate the use and point out differences in the interfaces, and close with some simple benchmarks comparing serialization/deserialization performance and sizes for several sample data structures.

The frameworks covered are:
* Apache AVRO
* Microsoft Bond
* Boost Serialization
* Cap'n'Proto
* Cereal
* Google Flatbuffers
* Google Protocol Buffers
* Apache Thrift
* ASN.1 (as a refrence, code not publicly available)


## SQL and C++ (german)

An overview of different database access APIs for C++, from native C SQLite3/PostgreSQL to SQLPP11. See 
the sample application below for code.

### Sample application: NotebookDatabase

This directory contains a Demo application showing different ways to interface with Sqlite / PostgreSQL 
databases from C++.

The demo covers
* SQLite C API
* PostgreSQL libpq API
* QtSql
* Wt::Dbo
* Sqlpp11 

All need the SQLite libraries and headers and a PostgreSQL libpq. Also the
Boost Date-Time headers and libs are needed.

For Qt: (this also includes libs for a Qt GUI client test)
* libqt5sql5, libqt5sql5-sqlite, libqt5sql5-
* libqt5quick5, libqt5qml5, libqt5gui5, libqt5declarative5, libqt5quickwidgets5

For Wt::Dbo:
* Ubuntu Packages: libwtdbo-dev, libwtdbopostgres-dev, libwtdbosqlite-dev
* Or code from http://www.webtoolkit.eu/wt

For Sqlpp11:
* Sqlpp11 main code from https://github.com/rbock/sqlpp11
* Sqlite Connector: https://github/com/rbock/sqlpp11-connector-sqlite3
* PostgreSQL Connector: https://github.com/matthijs/sqlpp11-connector-postgresql
* date.h from https://github.com/HowardHinnant/date

