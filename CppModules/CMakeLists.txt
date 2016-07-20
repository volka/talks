project(CppModules)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "/experimental:module")

add_executable(modules_test main.cpp module.ixx)
