#include "benchmark/benchmark_api.h"
#include <arpa/inet.h>
#include <functional>
#include <iostream>
#include <vector>

#include "proto3/serialize.pb.h"

#include "datasets.h"

#include <stdlib.h>

using generatorFunc = std::function<size_t(unsigned char*, size_t, int)>;

// my local configuraition object, used by fixture
static std::vector<std::tuple<size_t, generatorFunc, std::vector<unsigned char>*>> configuration;

// generaic reader and writer functions executed in the benchmark for each element
template <class T>
uint16_t writeProtoMsg(unsigned char* buf, size_t bufSize, const T& msg)
{
   uint16_t current_size = msg.ByteSize();
   if (bufSize < static_cast<size_t>(current_size + 2))
      throw std::runtime_error("Error serializing ints structure, insufficient buffer size");

   uint16_t network_current_size = htons(current_size);
   memcpy(buf, (char*)&network_current_size, sizeof(network_current_size));

   bool result = msg.SerializeToArray(buf + 2, bufSize);
   if (!result)
      throw std::runtime_error("Error serializing ints structure");

   return current_size + 2;
}

template <class T>
uint16_t readProtoMsg(unsigned char* buf, size_t bufSize, T& msg)
{
   uint16_t current_size;
   if (bufSize < 2)
      throw std::runtime_error("Insufficient buffer size for deserialization, no item left");
   memcpy(&current_size, buf, sizeof(current_size));
   current_size = ntohs(current_size);
   if (bufSize < static_cast<size_t>(current_size + 2))
      throw std::runtime_error("Error parsing item from array, insufficient buffer size");
   if (!msg.ParseFromArray(buf + 2, current_size))
      throw std::runtime_error("Error parsing item from array");
   return current_size + 2;
}

// use for both serialize and deserialize tests, specialize by overwriting constructor
template <int CONFIG_NO>
class BufferFixture : public ::benchmark::Fixture
{
 protected:
   std::vector<unsigned char>* buffer;
   // generator function to generate(void* buffer, size_t buffer_size, int generatedMessageCount)
   generatorFunc generator;
   int msgCount;

 public:
   BufferFixture() {}
   virtual ~BufferFixture() = default;

   virtual void SetUp(const benchmark::State&) override
   {
      setCount(std::get<0>(configuration[CONFIG_NO]));
      setGenerator(std::get<1>(configuration[CONFIG_NO]));
      setBuffer(std::get<2>(configuration[CONFIG_NO]));
      generate();
   }

   virtual void TearDown(const benchmark::State&) override {}

   size_t generate() { return generator(buffer->data(), buffer->size(), msgCount); }
   void setGenerator(generatorFunc gen) { generator = gen; }
   void setCount(int count) { msgCount = count; }
   void setBuffer(std::vector<unsigned char>* buf) { buffer = buf; }

   void runSerialize(generatorFunc func, benchmark::State& state)
   {
      size_t bytes = 0;
      setGenerator(func);
      while (state.KeepRunning())
      {
         bytes += generate();
      }
      state.SetBytesProcessed(bytes);
      state.SetItemsProcessed(state.iterations() * msgCount);

      printSize(state, bytes, msgCount);
   }

   template <class Type>
   void runDeserialize(benchmark::State& state)
   {
      std::vector<Type> result_vec(msgCount);
      uint16_t current_size = 0;
      size_t bytes = 0;
      while (state.KeepRunning())
      {
         unsigned char* buf = buffer->data();
         size_t bufSize = buffer->size();
         for (auto& current : result_vec)
         {
            current_size = readProtoMsg(buf, bufSize, current);
            bufSize -= current_size;
            buf += current_size;
            bytes += current_size;
         }
      }
      state.SetBytesProcessed(bytes);
      state.SetItemsProcessed(state.iterations() * msgCount);

      printSize(state, bytes, msgCount);
   }
};

namespace
{

// generator functions
size_t generateNoop(unsigned char*, size_t, int) { return 0; }

size_t generateInts(int int_val, unsigned char* buffer, size_t bufSize, int count)
{
   size_t generated = 0;
   uint16_t current_size = 0;
   unsigned char* charBuf = buffer;
   serialize::Ints current;
   for (int i = 0; i < count; ++i)
   {
      current.Clear();
      current.set_id(i);
      current.set_int1(int_val++);
      current.set_int2(int_val + 1);
      current.set_int3(int_val + i);
      current.set_int4(int_val + i + 40);

      current_size = writeProtoMsg(charBuf, bufSize, current);

      bufSize -= current_size;
      charBuf += current_size;
      generated += current_size;
   }
   return generated;
}

size_t generateMixed(unsigned char* buffer, size_t bufSize, int count)
{
   size_t generated = 0;
   uint16_t current_size = 0;
   unsigned char* charBuf = buffer;
   int int_val = 123456;
   serialize::Mixed current;
   for (int i = 0; i < count; ++i)
   {
      current.Clear();
      current.set_id(i);
      current.set_int1(int_val + 10);
      current.set_int2(int_val++);
      current.set_uint1(2);
      current.set_float1(3.14);
      current.set_enum1((i % 2 == 0) ? ::serialize::ENUMERATION::ONE
                                     : ::serialize::ENUMERATION::TWO);
      current.set_text1(string_short);
      current_size = writeProtoMsg(charBuf, bufSize, current);

      bufSize -= current_size;
      charBuf += current_size;
      generated += current_size;
   }
   return generated;
}

size_t generateContainerVec(unsigned char* buffer, size_t bufSize, int count)
{
   size_t generated = 0;
   uint16_t current_size = 0;
   unsigned char* charBuf = buffer;
   serialize::Container_Vec current;
   int int_val = 123456;
   int cont_count = 50;
   for (int i = 0; i < count; ++i)
   {
      current.Clear();
      current.set_id(i);
      current.set_int1(int_val++);
      current.set_text1(string_short);
      for (int j = 0; j < cont_count; ++j)
      {
         current.add_stringvec(string_short);
         current.add_intvec(int_val);
      }

      current_size = writeProtoMsg(charBuf, bufSize, current);

      bufSize -= current_size;
      charBuf += current_size;
      generated += current_size;
   }
   return generated;
}

size_t generateContainerMap(unsigned char* buffer, size_t bufSize, int count)
{
   size_t generated = 0;
   uint16_t current_size = 0;
   unsigned char* charBuf = buffer;
   serialize::Container_Map current;
   int int_val = 123456;
   int cont_count = 20;
   for (int i = 0; i < count; ++i)
   {
      current.Clear();
      current.set_id(i);
      current.set_int1(int_val++);
      current.set_text1(string_short);
      for (int j = 0; j < cont_count; ++j)
      {
         (*current.mutable_map1())[j] = string_short;
      }
      current_size = writeProtoMsg(charBuf, bufSize, current);

      bufSize -= current_size;
      charBuf += current_size;
      generated += current_size;
   }
   return generated;
}

size_t generateComplex(unsigned char* buffer, size_t bufSize, int count)
{
   size_t generated = 0;
   uint16_t current_size = 0;
   unsigned char* charBuf = buffer;
   serialize::Complex current;
   int int_val = 123456;
   for (int i = 0; i < count; ++i)
   {
      current.Clear();
      current.set_id(i);
      current.set_int1(int_val * int_val);
      current.set_int2(int_val * 2);
      current.set_int3(int_val);
      current.set_int4(int_val % 100);
      current.set_int5(-int_val);
      current.set_int6(int_val++);
      current.set_enum1((i % 2 == 0) ? ::serialize::ENUMERATION::ONE
                                     : ::serialize::ENUMERATION::TWO);
      current.set_float1(3.14 * i);
      current.set_float2(3.14 * int_val);
      current.set_uint1(static_cast<uint32_t>(int_val));
      current.set_uint2(static_cast<uint64_t>(int_val * int_val));
      current.set_text1(string_short + std::to_string(i));
      current.set_text2(string_short + string_short);
      current.set_text3(string_long);
      current.set_text4(string_short);
      current.set_text5(string_long + std::to_string(int_val));

      current_size = writeProtoMsg(charBuf, bufSize, current);

      bufSize -= current_size;
      charBuf += current_size;
      generated += current_size;
   }
   return generated;
}

using namespace std::placeholders;

constexpr size_t MAX_BUF_SIZE = 8 * 1024 * 1024;
std::vector<unsigned char> global_buffer;

void setupConfiguration()
{
   global_buffer.resize(MAX_BUF_SIZE);
   configuration.push_back(std::make_tuple(1000, generateNoop, &global_buffer));
   configuration.push_back(std::make_tuple(10000, generateNoop, &global_buffer));
   configuration.push_back(
       std::make_tuple(1000, std::bind(generateInts, small_int, _1, _2, _3), &global_buffer));
   configuration.push_back(
       std::make_tuple(10000, std::bind(generateInts, small_int, _1, _2, _3), &global_buffer));
   configuration.push_back(
       std::make_tuple(1000, std::bind(generateInts, integer, _1, _2, _3), &global_buffer));
   configuration.push_back(
       std::make_tuple(10000, std::bind(generateInts, integer, _1, _2, _3), &global_buffer));
   configuration.push_back(std::make_tuple(1000, generateMixed, &global_buffer));
   configuration.push_back(std::make_tuple(10000, generateMixed, &global_buffer));
   configuration.push_back(std::make_tuple(1000, generateContainerVec, &global_buffer));
   configuration.push_back(std::make_tuple(10000, generateContainerVec, &global_buffer));
   configuration.push_back(std::make_tuple(1000, generateContainerMap, &global_buffer));
   configuration.push_back(std::make_tuple(10000, generateContainerMap, &global_buffer));
   configuration.push_back(std::make_tuple(1000, generateComplex, &global_buffer));
   configuration.push_back(std::make_tuple(10000, generateComplex, &global_buffer));
}
}

// Setup fixtures for configurations
using SerInts1000 = BufferFixture<0>;
using DeSerInts1000 = BufferFixture<2>;
using SerInts10000 = BufferFixture<1>;
using DeSerInts10000 = BufferFixture<3>;

using SerLargeInts1000 = BufferFixture<0>;
using DeSerLargeInts1000 = BufferFixture<4>;
using SerLargeInts10000 = BufferFixture<1>;
using DeSerLargeInts10000 = BufferFixture<5>;

using SerMixed1000 = BufferFixture<0>;
using DeSerMixed1000 = BufferFixture<6>;
using SerMixed10000 = BufferFixture<1>;
using DeSerMixed10000 = BufferFixture<7>;

using SerVec1000 = BufferFixture<0>;
using DeSerVec1000 = BufferFixture<8>;
using SerVec10000 = BufferFixture<1>;
using DeSerVec10000 = BufferFixture<9>;

using SerVec1000 = BufferFixture<0>;
using DeSerVec1000 = BufferFixture<8>;
using SerVec10000 = BufferFixture<1>;
using DeSerVec10000 = BufferFixture<9>;

using SerMap1000 = BufferFixture<0>;
using DeSerMap1000 = BufferFixture<10>;
using SerMap10000 = BufferFixture<1>;
using DeSerMap10000 = BufferFixture<11>;

using SerComplex1000 = BufferFixture<0>;
using DeSerComplex1000 = BufferFixture<12>;
using SerComplex10000 = BufferFixture<1>;
using DeSerComplex10000 = BufferFixture<13>;

BENCHMARK_F(SerInts1000, proto3_ints_serialize)(benchmark::State& state)
{
   runSerialize(std::bind(generateInts, small_int, _1, _2, _3), state);
}

BENCHMARK_F(DeSerInts1000, proto3_ints_deserialize)(benchmark::State& state)
{
   runDeserialize<serialize::Ints>(state);
}

BENCHMARK_F(SerInts10000, proto3_ints_serialize2)(benchmark::State& state)
{
   runSerialize(std::bind(generateInts, small_int, _1, _2, _3), state);
}

BENCHMARK_F(DeSerInts10000, proto3_ints_deserialize2)(benchmark::State& state)
{
   runDeserialize<serialize::Ints>(state);
}

BENCHMARK_F(SerLargeInts1000, proto3_largeints_serialize)(benchmark::State& state)
{
   runSerialize(std::bind(generateInts, integer, _1, _2, _3), state);
}

BENCHMARK_F(DeSerLargeInts1000, proto3_largeints_deserialize)(benchmark::State& state)
{
   runDeserialize<serialize::Ints>(state);
}

BENCHMARK_F(SerLargeInts10000, proto3_largeints_serialize2)(benchmark::State& state)
{
   runSerialize(std::bind(generateInts, integer, _1, _2, _3), state);
}

BENCHMARK_F(DeSerLargeInts10000, proto3_largeints_deserialize2)(benchmark::State& state)
{
   runDeserialize<serialize::Ints>(state);
}

BENCHMARK_F(SerMixed1000, proto3_mixed_serialize)(benchmark::State& state)
{
   runSerialize(generateMixed, state);
}

BENCHMARK_F(DeSerMixed1000, proto3_mixed_deserialize)(benchmark::State& state)
{
   runDeserialize<serialize::Mixed>(state);
}

BENCHMARK_F(SerMixed10000, proto3_mixed_serialize2)(benchmark::State& state)
{
   runSerialize(generateMixed, state);
}

BENCHMARK_F(DeSerMixed10000, proto3_mixed_deserialize2)(benchmark::State& state)
{
   runDeserialize<serialize::Mixed>(state);
}

BENCHMARK_F(SerComplex1000, proto3_complex_serialize)(benchmark::State& state)
{
   runSerialize(generateComplex, state);
}

BENCHMARK_F(DeSerComplex1000, proto3_complex_deserialize)(benchmark::State& state)
{
   runDeserialize<serialize::Complex>(state);
}

BENCHMARK_F(SerComplex10000, proto3_complex_serialize2)(benchmark::State& state)
{
   runSerialize(generateComplex, state);
}

BENCHMARK_F(DeSerComplex10000, proto3_complex_deserialize2)(benchmark::State& state)
{
   runDeserialize<serialize::Complex>(state);
}
BENCHMARK_F(SerVec1000, proto3_vec_serialize)(benchmark::State& state)
{
   runSerialize(generateContainerVec, state);
}

BENCHMARK_F(DeSerVec1000, proto3_vec_deserialize)(benchmark::State& state)
{
   runDeserialize<serialize::Container_Vec>(state);
}

BENCHMARK_F(SerVec10000, proto3_vec_serialize2)(benchmark::State& state)
{
   runSerialize(generateContainerVec, state);
}

BENCHMARK_F(DeSerVec10000, proto3_vec_deserialize2)(benchmark::State& state)
{
   runDeserialize<serialize::Container_Vec>(state);
}
BENCHMARK_F(SerMap1000, proto3_map_serialize)(benchmark::State& state)
{
   runSerialize(generateContainerMap, state);
}

BENCHMARK_F(DeSerMap1000, proto3_map_deserialize)(benchmark::State& state)
{
   runDeserialize<serialize::Container_Map>(state);
}

BENCHMARK_F(SerMap10000, proto3_map_serialize2)(benchmark::State& state)
{
   runSerialize(generateContainerMap, state);
}

BENCHMARK_F(DeSerMap10000, proto3_map_deserialize2)(benchmark::State& state)
{
   runDeserialize<serialize::Container_Map>(state);
}

int main(int argc, char** argv)
{
   setupConfiguration();
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}
