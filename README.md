# talks
Slides and demo code for talks ...

## Security Basics for CPP Developers (short talk)

The talk can be found in "CppSecurity.pdf"

The CppSecurityBasics directory contains demo code for doing security-related things in a reasonable
secure way even if you do not know a lot about security. The examples work with OpenSSL.

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

## CPP Serialization (Pack ein pack aus, german)

This is an evaluation of lots of different serialization frameworks for C++, mainly comparing usage,
performance and sizes for several sample datasets. Slides for the talk can be found in 2017\_01\_Cpp\_Serialization.pdf

The evaluation covers the following toolkits:

* Apache AVRO
* Microsoft Bond
* Boost Serialization
* Cereal
* Cap'n'Proto
* Apache Thrift
* Google Protobuf
* Google Flatbuffers
* ASN.1 (internal library removed for public release)

Demo code can be found in the CppSerialization subdirectory. The code uses the Google Benchmark library to
testdrive the frameworks. It needs some setup of the respective libraries, and tuning of the CMakeLists.txt
file for the respective framework.
