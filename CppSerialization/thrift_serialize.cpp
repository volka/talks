#include "benchmark/benchmark_api.h"
#include <arpa/inet.h>
#include <functional>
#include <iostream>
#include <vector>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/protocol/TProtocol.h>

#include <thrift/transport/TBufferTransports.h>

#include "thrift/serialize_constants.h"
#include "thrift/serialize_types.h"

#include "datasets.h"

using namespace apache::thrift;
using namespace std::placeholders;

constexpr size_t MAX_BUF_SIZE = 8 * 1024 * 1024;
static std::vector<unsigned char> global_buffer;

void setupConfiguration() { global_buffer.resize(MAX_BUF_SIZE); }
using generatorFunc = std::function<size_t(protocol::TProtocol*, int)>;

// generator functions
size_t generateNoop(protocol::TProtocol*, int) { return 0; }

size_t generateInts(int int_val, protocol::TProtocol* out, int count)
{
   size_t generated = 0;

   thrift_serialize::ints current;
   for (int i = 0; i < count; ++i)
   {
      current.id = i;
      current.int1 = int_val++;
      current.int2 = int_val + 1;
      current.int3 = int_val + i;
      current.int4 = int_val + i + 40;

      generated += current.write(out);
   }
   out->getTransport()->flush();
   return generated;
}

size_t generateMixed(protocol::TProtocol* out, int count)
{
   size_t generated = 0;
   int int_val = 123456;
   thrift_serialize::mixed current;
   for (int i = 0; i < count; ++i)
   {
      current.id = i;
      current.int1 = int_val + 10;
      current.int2 = int_val++;
      current.int3 = 2;
      current.float1 = 3.14;
      current.enum1 = (i % 2 == 0) ? ::thrift_serialize::ENUMERATION::ONE
                                   : ::thrift_serialize::ENUMERATION::TWO;
      current.text1 = string_short;
      generated += current.write(out);
   }
   out->getTransport()->flush();
   return generated;
}

size_t generateContainerVec(protocol::TProtocol* out, int count)
{
   size_t generated = 0;
   thrift_serialize::container_vec current;

   int int_val = 123456;
   int cont_count = 50;

   for (int i = 0; i < count; ++i)
   {
      current.stringvec.clear();
      current.intvec.clear();

      current.id = i;
      current.int1 = int_val++;
      current.text1 = string_short;

      for (int j = 0; j < cont_count; ++j)
      {
         current.stringvec.push_back(string_short);
         current.intvec.push_back(int_val);
      }

      generated += current.write(out);
   }
   out->getTransport()->flush();
   return generated;
}

size_t generateContainerMap(protocol::TProtocol* out, int count)
{
   size_t generated = 0;
   thrift_serialize::container_map current;
   int int_val = 123456;
   int cont_count = 20;

   for (int i = 0; i < count; ++i)
   {
      current.map1.clear();

      current.id = i;
      current.int1 = int_val++;
      current.text1 = string_short;
      for (int j = 0; j < cont_count; ++j)
      {
         current.map1[j] = string_short;
      }
      generated += current.write(out);
   }
   out->getTransport()->flush();
   return generated;
}

size_t generateComplex(protocol::TProtocol* out, int count)
{
   size_t generated = 0;
   thrift_serialize::complex current;
   int int_val = 123456;

   for (int i = 0; i < count; ++i)
   {
      current.id = i;
      current.int1 = int_val * int_val;
      current.int2 = int_val * 2;
      current.int3 = int_val;
      current.int4 = int_val % 100;
      current.int5 = -int_val;
      current.int6 = int_val++;
      current.enum1 = (i % 2 == 0) ? ::thrift_serialize::ENUMERATION::ONE
                                   : ::thrift_serialize::ENUMERATION::TWO;
      current.float1 = 3.14 * i;
      current.float2 = 3.14 * int_val;
      current.uint1 = static_cast<uint32_t>(int_val);
      current.uint2 = static_cast<uint64_t>(int_val * int_val);
      current.text1 = string_short + std::to_string(i);
      current.text2 = string_short + string_short;
      current.text3 = string_long;
      current.text4 = string_short;
      current.text5 = string_long + std::to_string(int_val);

      generated += current.write(out);
   }
   out->getTransport()->flush();
   return generated;
}

template <class PROTO>
int runSerialize(benchmark::State& st, int msgCount, generatorFunc generate)
{
   auto buffer = boost::make_shared<transport::TMemoryBuffer>(MAX_BUF_SIZE);
   //            global_buffer.data(), global_buffer.size(), transport::TMemoryBuffer::OBSERVE);
   auto transport = boost::make_shared<transport::TBufferedTransport>(buffer);
   auto proto = boost::make_shared<PROTO>(transport);
   size_t generated = 0;
   while (st.KeepRunning())
   {
      generated += generate(proto.get(), msgCount);
   }
   st.SetBytesProcessed(generated);
   st.SetItemsProcessed(st.iterations() * msgCount);
   printSize(st, generated, msgCount);

   return generated;
}

template <class PROTO, class MsgType>
int runDeSerialize(benchmark::State& st, int msgCount, generatorFunc generate)
{
   std::vector<MsgType> result(msgCount);

   // this is the buffer we generate our test-data into
   boost::shared_ptr<transport::TMemoryBuffer> in_buffer =
       boost::make_shared<transport::TMemoryBuffer>();
   auto transport = boost::make_shared<transport::TBufferedTransport>(in_buffer);
   auto proto = boost::make_shared<PROTO>(transport);
   generate(proto.get(), msgCount);

   // we cannot access the write buffer directly, so we have to jump through some hoops
   uint8_t* raw_buf = nullptr;
   uint32_t raw_size = 0;
   in_buffer->getBuffer(&raw_buf, &raw_size);

   size_t bytes_read = 0;
   while (st.KeepRunning())
   {
      // reset transport and proto, create new read buffer from raw pointers
      auto out_buffer = boost::make_shared<transport::TMemoryBuffer>(
          raw_buf, raw_size, transport::TMemoryBuffer::OBSERVE);
      transport = boost::make_shared<transport::TBufferedTransport>(out_buffer);
      proto = boost::make_shared<PROTO>(transport);

      for (auto& current : result)
      {
         bytes_read += current.read(proto.get());
      }
   }

   st.SetBytesProcessed(bytes_read);
   st.SetItemsProcessed(st.iterations() * msgCount);
   printSize(st, bytes_read, msgCount);
   return bytes_read;
}

template <class PROTO, size_t COUNT, int INT_VAL>
void serialize_ints_thrift(benchmark::State& st)
{
   runSerialize<PROTO>(st, COUNT, std::bind(generateInts, INT_VAL, _1, _2));
}

template <class PROTO, size_t COUNT, int INT_VAL>
void deserialize_ints_thrift(benchmark::State& st)
{
   runDeSerialize<PROTO, thrift_serialize::ints>(st, COUNT,
                                                 std::bind(generateInts, INT_VAL, _1, _2));
}

template <class PROTO, size_t COUNT>
void serialize_mixed_thrift(benchmark::State& st)
{
   runSerialize<PROTO>(st, COUNT, generateMixed);
}

template <class PROTO, size_t COUNT>
void deserialize_mixed_thrift(benchmark::State& st)
{
   runDeSerialize<PROTO, thrift_serialize::mixed>(st, COUNT, generateMixed);
}

template <class PROTO, size_t COUNT>
void serialize_container_vec_thrift(benchmark::State& st)
{
   runSerialize<PROTO>(st, COUNT, generateContainerVec);
}

template <class PROTO, size_t COUNT>
void deserialize_container_vec_thrift(benchmark::State& st)
{
   runDeSerialize<PROTO, thrift_serialize::container_vec>(st, COUNT, generateContainerVec);
}

template <class PROTO, size_t COUNT>
void serialize_container_map_thrift(benchmark::State& st)
{
   runSerialize<PROTO>(st, COUNT, generateContainerMap);
}

template <class PROTO, size_t COUNT>
void deserialize_container_map_thrift(benchmark::State& st)
{
   runDeSerialize<PROTO, thrift_serialize::container_map>(st, COUNT, generateContainerMap);
}

template <class PROTO, size_t COUNT>
void serialize_complex_thrift(benchmark::State& st)
{
   runSerialize<PROTO>(st, COUNT, generateComplex);
}

template <class PROTO, size_t COUNT>
void deserialize_complex_thrift(benchmark::State& st)
{
   runDeSerialize<PROTO, thrift_serialize::complex>(st, COUNT, generateComplex);
}

BENCHMARK_TEMPLATE(serialize_ints_thrift, protocol::TBinaryProtocol, 1000, small_int);
BENCHMARK_TEMPLATE(serialize_ints_thrift, protocol::TBinaryProtocol, 10000, small_int);
BENCHMARK_TEMPLATE(serialize_ints_thrift, protocol::TBinaryProtocol, 1000, integer);
BENCHMARK_TEMPLATE(serialize_ints_thrift, protocol::TBinaryProtocol, 10000, integer);
BENCHMARK_TEMPLATE(serialize_ints_thrift, protocol::TCompactProtocol, 1000, small_int);
BENCHMARK_TEMPLATE(serialize_ints_thrift, protocol::TCompactProtocol, 10000, small_int);
BENCHMARK_TEMPLATE(serialize_ints_thrift, protocol::TCompactProtocol, 1000, integer);
BENCHMARK_TEMPLATE(serialize_ints_thrift, protocol::TCompactProtocol, 10000, integer);
BENCHMARK_TEMPLATE(serialize_ints_thrift, protocol::TJSONProtocol, 1000, small_int);
BENCHMARK_TEMPLATE(serialize_ints_thrift, protocol::TJSONProtocol, 10000, small_int);

BENCHMARK_TEMPLATE(deserialize_ints_thrift, protocol::TBinaryProtocol, 1000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_thrift, protocol::TBinaryProtocol, 10000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_thrift, protocol::TBinaryProtocol, 1000, integer);
BENCHMARK_TEMPLATE(deserialize_ints_thrift, protocol::TBinaryProtocol, 10000, integer);
BENCHMARK_TEMPLATE(deserialize_ints_thrift, protocol::TCompactProtocol, 1000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_thrift, protocol::TCompactProtocol, 10000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_thrift, protocol::TCompactProtocol, 1000, integer);
BENCHMARK_TEMPLATE(deserialize_ints_thrift, protocol::TCompactProtocol, 10000, integer);
BENCHMARK_TEMPLATE(deserialize_ints_thrift, protocol::TJSONProtocol, 1000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_thrift, protocol::TJSONProtocol, 10000, small_int);

BENCHMARK_TEMPLATE(serialize_mixed_thrift, protocol::TBinaryProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_thrift, protocol::TBinaryProtocol, 10000);
BENCHMARK_TEMPLATE(serialize_mixed_thrift, protocol::TCompactProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_thrift, protocol::TCompactProtocol, 10000);
BENCHMARK_TEMPLATE(serialize_mixed_thrift, protocol::TJSONProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_thrift, protocol::TJSONProtocol, 10000);

BENCHMARK_TEMPLATE(deserialize_mixed_thrift, protocol::TBinaryProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_thrift, protocol::TBinaryProtocol, 10000);
BENCHMARK_TEMPLATE(deserialize_mixed_thrift, protocol::TCompactProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_thrift, protocol::TCompactProtocol, 10000);
BENCHMARK_TEMPLATE(deserialize_mixed_thrift, protocol::TJSONProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_thrift, protocol::TJSONProtocol, 10000);

BENCHMARK_TEMPLATE(serialize_container_vec_thrift, protocol::TBinaryProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_container_vec_thrift, protocol::TBinaryProtocol, 10000);
BENCHMARK_TEMPLATE(serialize_container_vec_thrift, protocol::TCompactProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_container_vec_thrift, protocol::TCompactProtocol, 10000);
BENCHMARK_TEMPLATE(serialize_container_vec_thrift, protocol::TJSONProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_container_vec_thrift, protocol::TJSONProtocol, 10000);

BENCHMARK_TEMPLATE(deserialize_container_vec_thrift, protocol::TBinaryProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_container_vec_thrift, protocol::TBinaryProtocol, 10000);
BENCHMARK_TEMPLATE(deserialize_container_vec_thrift, protocol::TCompactProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_container_vec_thrift, protocol::TCompactProtocol, 10000);
BENCHMARK_TEMPLATE(deserialize_container_vec_thrift, protocol::TJSONProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_container_vec_thrift, protocol::TJSONProtocol, 10000);

BENCHMARK_TEMPLATE(serialize_container_map_thrift, protocol::TBinaryProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_container_map_thrift, protocol::TBinaryProtocol, 10000);
BENCHMARK_TEMPLATE(serialize_container_map_thrift, protocol::TCompactProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_container_map_thrift, protocol::TCompactProtocol, 10000);
BENCHMARK_TEMPLATE(serialize_container_map_thrift, protocol::TJSONProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_container_map_thrift, protocol::TJSONProtocol, 10000);

BENCHMARK_TEMPLATE(deserialize_container_map_thrift, protocol::TBinaryProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_container_map_thrift, protocol::TBinaryProtocol, 10000);
BENCHMARK_TEMPLATE(deserialize_container_map_thrift, protocol::TCompactProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_container_map_thrift, protocol::TCompactProtocol, 10000);
BENCHMARK_TEMPLATE(deserialize_container_map_thrift, protocol::TJSONProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_container_map_thrift, protocol::TJSONProtocol, 10000);

BENCHMARK_TEMPLATE(serialize_complex_thrift, protocol::TBinaryProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_complex_thrift, protocol::TBinaryProtocol, 10000);
BENCHMARK_TEMPLATE(serialize_complex_thrift, protocol::TCompactProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_complex_thrift, protocol::TCompactProtocol, 10000);
BENCHMARK_TEMPLATE(serialize_complex_thrift, protocol::TJSONProtocol, 1000);
BENCHMARK_TEMPLATE(serialize_complex_thrift, protocol::TJSONProtocol, 10000);

BENCHMARK_TEMPLATE(deserialize_complex_thrift, protocol::TBinaryProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_thrift, protocol::TBinaryProtocol, 10000);
BENCHMARK_TEMPLATE(deserialize_complex_thrift, protocol::TCompactProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_thrift, protocol::TCompactProtocol, 10000);
BENCHMARK_TEMPLATE(deserialize_complex_thrift, protocol::TJSONProtocol, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_thrift, protocol::TJSONProtocol, 10000);

int main(int argc, char** argv)
{
   setupConfiguration();
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}
