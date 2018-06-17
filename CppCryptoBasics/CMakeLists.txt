cmake_minimum_required(VERSION 3.2)
project(CppSecurity)
find_package(OpenSSL)
find_package(Boost COMPONENTS system)

#set(CMAKE_CXX_COMPILER "clang++-4.0")
set(CMAKE_CXX_FLAGS "-std=c++1z -pthread -DSQLITE_HAS_CODEC")
link_directories("/usr/local/lib")
include_directories("~/code/GSL/include")
include_directories("/usr/local/include/botan-2")

add_executable(enc_sql enc_sql.cpp)

add_executable(ossl_random ossl_random.cpp util.cpp)
add_executable(ossl_hashing ossl_hashing.cpp util.cpp)
add_executable(ossl_encryption ossl_encryption.cpp util.cpp)

add_executable(botan_random botan_random.cpp)
add_executable(botan_hashing botan_hashing.cpp)
add_executable(botan_encryption botan_encryption.cpp)
#add_executable(botan_ssl botan_ssl.cpp)

target_link_libraries(ossl_random OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(ossl_hashing OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(ossl_encryption OpenSSL::SSL OpenSSL::Crypto)

target_link_libraries(botan_random botan-2)
target_link_libraries(botan_hashing botan-2)
target_link_libraries(botan_encryption botan-2)
#target_link_libraries(botan_ssl botan-2)

target_link_libraries(enc_sql sqlcipher)
#target_include_directories(ossl_random PUBLIC "~/code/GSL/include")
#target_include_directories(ossl_hashing PUBLIC "~/code/GSL/include")
#target_include_directories(ossl_encryption PUBLIC "~/code/GSL/include")
#target_include_directories(ossl_ssl PUBLIC "~/code/GSL/include")

#target_include_directories(botan_random PUBLIC "/usr/local/include/botan-2")
#target_include_directories(botan_hashing PUBLIC "/usr/local/include/botan-2")
#target_include_directories(botan_encryption PUBLIC "/usr/local/include/botan-2")
#target_include_directories(botan_ssl PUBLIC "/usr/local/include/botan-2")
