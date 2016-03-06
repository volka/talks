# talks
Slides and demo code for talks ...

## DbSample

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

