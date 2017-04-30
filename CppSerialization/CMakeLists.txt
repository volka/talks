cmake_minimum_required(VERSION 2.8)
project (serialize_test)

set( CMAKE_BUILD_TYPE Release )
set( BOOST_USE_STATIC_LIBS OFF )
#set( BOOST_ROOT "/opt/utimaco/thirdparty/boost-1.57.0" )
#set( BOOST_LIBRARYDIR "/opt/utimaco/thirdparty/boost-1.57.0/lib64" )
#set( BOOST_INCLUDEDIR "/opt/utimaco/thirdparty/boost-1.57.0/include" )
set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/lib/pkgconfig" )

find_package(PkgConfig)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS serialization REQUIRED)
find_package(ZLIB REQUIRED)

pkg_check_modules(PROTOBUF REQUIRED protobuf)
pkg_check_modules(THRIFT REQUIRED thrift)
pkg_check_modules(CAPNPROTO REQUIRED capnp)

set(CEREAL_INCLUDES "/usr/local/include")

# Protobuf generates files with unused parameters ...
set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic -Wextra -O2 -g -std=c++14 -Wno-unused-variable")
include_directories("." "/home/volker/code/benchmark/include" "bond/" "proto3/" "thrift/" "capnp/"
                    ${THRIFT_CFLAGS} ${CEREAL_INCLUDES} ${Boost_INCLUDE_DIRS})

### FIX THIS TO INCLUDE YOUR LOCAL LIB PATH, NEEDED FOR LIBICC_gnu.a !
LINK_DIRECTORIES(${BOOST_ROOT}/lib64 "/usr/local/lib" "/home/volker/code/serialize_test" "/home/assmann/code/serialize_test/")

FILE(GLOB_RECURSE INC_ALL "*.h")

add_executable(google_bench_demo google_bench_demo.cpp)
target_link_libraries(google_bench_demo benchmark ${CMAKE_THREAD_LIBS_INIT})

add_executable(serialize_bond bond_serialize.cpp bond/serialize_types.cpp bond/serialize_apply.cpp ${INC_ALL})
target_link_libraries(serialize_bond benchmark ${CMAKE_THREAD_LIBS_INIT})

# add_executable(serialize_avro avro_serialize.cpp)
# target_link_libraries(serialize_avro benchmark "libavrocpp_s.a" ${CMAKE_THREAD_LIBS_INIT})

#add_executable(serialize_asn1 asn1_serialize.cpp asn1/dx_berenc.cc asn1/dx_ber.cc)
#target_link_libraries(serialize_asn1 benchmark "libicc_gnu.a" ${CMAKE_THREAD_LIBS_INIT})

add_executable(serialize_proto3 proto3_serialize.cpp proto3/serialize.pb.cc ${INC_ALL})
target_link_libraries(serialize_proto3 benchmark ${PROTOBUF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#add_executable(serialize_proto3_arena proto3_serialize_arena.cpp proto3_arena/serialize.pb.cc ${INC_ALL})
#target_link_libraries(serialize_proto3_arena benchmark ${PROTOBUF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_executable(serialize_thrift thrift_serialize.cpp thrift/serialize_types.cpp thrift/serialize_constants.cpp ${INC_ALL})
target_link_libraries(serialize_thrift benchmark ${THRIFT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_executable(serialize_capnp capnp_serialize.cpp ${INC_ALL})
target_link_libraries(serialize_capnp benchmark ${CAPNPROTO_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_executable(serialize_cereal cereal_serialize.cpp)
target_link_libraries(serialize_cereal benchmark ${CMAKE_THREAD_LIBS_INIT})

add_executable(serialize_boost boost_serialize.cpp)
target_link_libraries(serialize_boost benchmark ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

#add_executable(serialize_flatbuf flatbuf_serialize.cpp)
#target_link_libraries(serialize_flatbuf benchmark "libflatbuffers.a" ${ZLIB_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
