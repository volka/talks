* Overview of SQL Libraries in C++
** Current state of SQL

* DB Libraries / Bindings
** Overview of DB Bindings
** SQLite3 API
** PostgreSQL API
*** Note on libpqtypes
** ( ODBC )

* Simple Text Wrappers
** QtSQL (low level)
*** Note SQLAPI

* C++ified Libs
** Soci
** Wt

* ORMs
** ODB
** QxOrm

* Our decision process: why did we not use any of those?
** Reqirements
 - Sybase Support (- ODB, SOCI, SQLAPI, Wt)
 - Easy, safe and secure to use (--ODBC, native APIs)
 - High performance (- ODBC)

* Template SQL Generators
** SQLPP11
*** Overall structure: templatized base functionality, connectors for DBs
*** Generating SQL with Expression templates
*** Contexts and serializers
*** 

* Sample App
** Simple note taking / todo tracker
** Category
** TODO Title
** Creation Date
** Due Date
