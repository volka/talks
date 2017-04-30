cmake_minimum_required(VERSION 3.2)

find_package(OpenSSL)

add_executable(ssl_demo ssl_demo.cpp)
add_executable(hashing_demo hashing_demo.cpp)
add_executable(encryption_demo encryption_demo.cpp)

target_link_libraries(ssl_demo OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(hashing_demo OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(encryption_demo OpenSSL::SSL OpenSSL::Crypto)
target_include_directories(ssl_demo PUBLIC "~/code/GSL/include")
target_include_directories(hashing_demo PUBLIC "~/code/GSL/include")
target_include_directories(encryption_demo PUBLIC "~/code/GSL/include")
