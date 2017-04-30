#include "datasets.h"
#include "serialize_reflection.h"

#include <strings.h>
#include <boost/shared_ptr.hpp>

#include <chrono>
#include <functional>
#include <iostream>

#include <bond/core/bond.h>
#include <bond/protocol/simple_json_reader.h>
#include <bond/protocol/simple_json_reader_impl.h>
#include <bond/protocol/simple_json_writer.h>
#include <bond/stream/output_buffer.h>

using namespace uti::serialize;

template <class Writer>
using generatorFunc = void (*)(Writer&, size_t);

constexpr static int MAX_BUF_SIZE = 40 * 1024 * 1024;
static boost::shared_ptr<char[]> global_buffer;

static void setup()
{
   global_buffer.reset(new char[MAX_BUF_SIZE], boost::checked_array_deleter<char>());
}

template <class Writer, int INT_VAL>
void generate_ints(Writer& writer, size_t count)
{
   ints current;
   size_t id = 0;
   int int_val = INT_VAL;

   while (id < count)
   {
      current.id = id++;
      current.int1 = int_val++;
      current.int2 = int_val + 1;
      current.int3 = int_val + id;
      current.int4 = int_val + id + 40;

      Serialize(current, writer);
   }
}

template <class Writer>
void generate_mixed(Writer& writer, size_t count)
{
   mixed m;
   int id = 0;
   int int_val = 123456789;

   while (id < static_cast<int>(count))
   {
      m.id = id++;
      m.int1 = int_val + 10;
      m.int2 = int_val++;
      m.float1 = 3.14;
      m.enum1 = (id % 2 == 0) ? Enum1::ONE : Enum1::TWO;
      m.uint1 = 2;
      m.text1 = string_short;

      Serialize(m, writer);
   }
}

template <class Writer>
void generate_contvec(Writer& writer, size_t count)
{
   container_vec current;
   int id = 0;
   int int_val = 123456;
   int cont_count = 50;
   while (id < static_cast<int>(count))
   {
      current.id = id++;
      current.int1 = int_val++;
      current.text1 = string_short;
      current.stringvec.clear();
      current.intvec.clear();
      for (int i = 0; i < cont_count; ++i)
      {
         current.stringvec.push_back(string_short);
         current.intvec.push_back(int_val);
      }
      Serialize(current, writer);
   }
}

template <class Writer>
void generate_contmap(Writer& writer, size_t count)
{
   container_map current;
   size_t id = 0;
   int int_val = 123456;
   int cont_count = 20;
   while (id < count)
   {
      current.map1.clear();
      current.id = static_cast<int>(id++);
      current.int1 = int_val++;
      current.text1 = string_short;
      for (int i = 0; i < cont_count; ++i)
      {
         current.map1.insert(std::make_pair(i, string_short));
      }
      Serialize(current, writer);
   }
}

template <class Writer>
void generate_complex(Writer& writer, size_t count)
{
   complex current;
   size_t id = 0;
   int int_val = 123456;
   while (id < count)
   {
      current.id = static_cast<int>(id++);
      current.int1 = int_val * int_val;
      current.int2 = int_val * 2;
      current.int3 = int_val;
      current.int4 = int_val % 100;
      current.int5 = -int_val;
      current.int6 = int_val++;
      current.enum1 = (id % 2 == 0) ? Enum1::ONE : Enum1::TWO;
      current.float1 = 3.14 * id;
      current.float2 = 3.14 * int_val;
      current.uint1 = static_cast<uint32_t>(int_val);
      current.uint2 = static_cast<uint64_t>(int_val * int_val);
      current.text1 = string_short + std::to_string(id);
      current.text2 = string_short + string_short;
      current.text3 = string_long;
      current.text4 = string_short;
      current.text5 = string_long + std::to_string(int_val);

      Serialize(current, writer);
   }
}

template <class Writer, class MsgType>
void runSerialize(benchmark::State& st, size_t count, generatorFunc<Writer> generator)
{
   size_t generated = 0;

   while (st.KeepRunning())
   {
      bzero(global_buffer.get(), MAX_BUF_SIZE);
      bond::OutputBuffer output(global_buffer, MAX_BUF_SIZE);
      Writer writer(output);
      generator(writer, count);
      generated += output.GetBuffer().size();
   }
   st.SetBytesProcessed(generated);
   st.SetItemsProcessed(st.iterations() * count);
   printSize(st, generated, count);
}

template <class Writer, class Reader, class MsgType>
void runDeSerialize(benchmark::State& st, size_t count, generatorFunc<Writer> generator)
{
   size_t bytes = 0;
   std::vector<MsgType> result(count);
   bond::blob data;
   {
      bond::OutputBuffer output(global_buffer, MAX_BUF_SIZE);
      Writer writer(output);
      generator(writer, count);
      data = output.GetBuffer();
      bytes = data.size();
   }
   while (st.KeepRunning())
   {
      Reader reader(data);
      for (auto& current : result)
      {
         Deserialize(reader, current);
      }
   }
   st.SetBytesProcessed(bytes * st.iterations());
   st.SetItemsProcessed(count * st.iterations());
   printSize(st, bytes * st.iterations(), count);
}

template <class Writer, size_t COUNT, int INT_VAL>
void serialize_ints_bond(benchmark::State& st)
{
   generatorFunc<Writer> gen = generate_ints<Writer, INT_VAL>;
   runSerialize<Writer, ints>(st, COUNT, gen);
}

template <class Writer, class Reader, size_t COUNT, int INT_VAL>
void deserialize_ints_bond(benchmark::State& st)
{
   generatorFunc<Writer> gen = generate_ints<Writer, INT_VAL>;
   runDeSerialize<Writer, Reader, ints>(st, COUNT, gen);
}

template <class Writer, size_t COUNT>
void serialize_mixed_bond(benchmark::State& st)
{
   runSerialize<Writer, mixed>(st, COUNT, generate_mixed);
}

template <class Writer, class Reader, size_t COUNT>
void deserialize_mixed_bond(benchmark::State& st)
{
   runDeSerialize<Writer, Reader, mixed>(st, COUNT, generate_mixed);
}

template <class Writer, size_t COUNT>
void serialize_complex_bond(benchmark::State& st)
{
   runSerialize<Writer, complex>(st, COUNT, generate_complex);
}

template <class Writer, class Reader, size_t COUNT>
void deserialize_complex_bond(benchmark::State& st)
{
   runDeSerialize<Writer, Reader, complex>(st, COUNT, generate_complex);
}

template <class Writer, size_t COUNT>
void serialize_contvec_bond(benchmark::State& st)
{
   runSerialize<Writer, container_vec>(st, COUNT, generate_contvec);
}

template <class Writer, class Reader, size_t COUNT>
void deserialize_contvec_bond(benchmark::State& st)
{
   runDeSerialize<Writer, Reader, container_vec>(st, COUNT, generate_contvec);
}

template <class Writer, size_t COUNT>
void serialize_contmap_bond(benchmark::State& st)
{
   runSerialize<Writer, container_map>(st, COUNT, generate_contmap);
}

template <class Writer, class Reader, size_t COUNT>
void deserialize_contmap_bond(benchmark::State& st)
{
   runDeSerialize<Writer, Reader, container_map>(st, COUNT, generate_contmap);
}

using FastWriter = bond::FastBinaryWriter<bond::OutputBuffer>;
using CompactWriter = bond::CompactBinaryWriter<bond::OutputBuffer>;
using JsonWriter = bond::SimpleJsonWriter<bond::OutputBuffer>;
using SimpleWriter = bond::SimpleBinaryWriter<bond::OutputBuffer>;
using FastReader = bond::FastBinaryReader<bond::InputBuffer>;
using CompactReader = bond::CompactBinaryReader<bond::InputBuffer>;
using JsonReader = bond::SimpleJsonReader<bond::InputBuffer>;
using SimpleReader = bond::SimpleBinaryReader<bond::InputBuffer>;


BENCHMARK_TEMPLATE(serialize_ints_bond, FastWriter, 1000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_bond, FastWriter, FastReader, 1000, small_int);
BENCHMARK_TEMPLATE(serialize_ints_bond, FastWriter, 10000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_bond, FastWriter, FastReader, 10000, small_int);

BENCHMARK_TEMPLATE(serialize_ints_bond, SimpleWriter, 1000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_bond, SimpleWriter, SimpleReader, 1000, small_int);
BENCHMARK_TEMPLATE(serialize_ints_bond, SimpleWriter, 10000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_bond, SimpleWriter, SimpleReader, 10000, small_int);

BENCHMARK_TEMPLATE(serialize_ints_bond, CompactWriter, 1000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_bond, CompactWriter, CompactReader, 1000, small_int);
BENCHMARK_TEMPLATE(serialize_ints_bond, CompactWriter, 10000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_bond, CompactWriter, CompactReader, 10000, small_int);

BENCHMARK_TEMPLATE(serialize_ints_bond, CompactWriter, 1000, integer);
BENCHMARK_TEMPLATE(deserialize_ints_bond, CompactWriter, CompactReader, 1000, integer);
BENCHMARK_TEMPLATE(serialize_ints_bond, CompactWriter, 10000, integer);
BENCHMARK_TEMPLATE(deserialize_ints_bond, CompactWriter, CompactReader, 10000, integer);

BENCHMARK_TEMPLATE(serialize_ints_bond, JsonWriter, 1000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_bond, JsonWriter, JsonReader, 1000, small_int);
BENCHMARK_TEMPLATE(serialize_ints_bond, JsonWriter, 10000, small_int);
BENCHMARK_TEMPLATE(deserialize_ints_bond, JsonWriter, JsonReader, 10000, small_int);

BENCHMARK_TEMPLATE(serialize_mixed_bond, FastWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_bond, FastWriter, FastReader, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_bond, FastWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_mixed_bond, FastWriter, FastReader, 10000);

BENCHMARK_TEMPLATE(serialize_mixed_bond, SimpleWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_bond, SimpleWriter, SimpleReader, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_bond, SimpleWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_mixed_bond, SimpleWriter, SimpleReader, 10000);

BENCHMARK_TEMPLATE(serialize_mixed_bond, CompactWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_bond, CompactWriter, CompactReader, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_bond, CompactWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_mixed_bond, CompactWriter, CompactReader, 10000);

BENCHMARK_TEMPLATE(serialize_mixed_bond, JsonWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_bond, JsonWriter, JsonReader, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_bond, JsonWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_mixed_bond, JsonWriter, JsonReader, 10000);

BENCHMARK_TEMPLATE(serialize_complex_bond, FastWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_bond, FastWriter, FastReader, 1000);
BENCHMARK_TEMPLATE(serialize_complex_bond, FastWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_complex_bond, FastWriter, FastReader, 10000);

BENCHMARK_TEMPLATE(serialize_complex_bond, CompactWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_bond, CompactWriter, CompactReader, 1000);
BENCHMARK_TEMPLATE(serialize_complex_bond, CompactWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_complex_bond, CompactWriter, CompactReader, 10000);

BENCHMARK_TEMPLATE(serialize_complex_bond, JsonWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_bond, JsonWriter, JsonReader, 1000);
BENCHMARK_TEMPLATE(serialize_complex_bond, JsonWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_complex_bond, JsonWriter, JsonReader, 10000);

BENCHMARK_TEMPLATE(serialize_contvec_bond, FastWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_contvec_bond, FastWriter, FastReader, 1000);
BENCHMARK_TEMPLATE(serialize_contvec_bond, FastWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_contvec_bond, FastWriter, FastReader, 10000);

BENCHMARK_TEMPLATE(serialize_contvec_bond, CompactWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_contvec_bond, CompactWriter, CompactReader, 1000);
BENCHMARK_TEMPLATE(serialize_contvec_bond, CompactWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_contvec_bond, CompactWriter, CompactReader, 10000);

BENCHMARK_TEMPLATE(serialize_contvec_bond, JsonWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_contvec_bond, JsonWriter, JsonReader, 1000);
BENCHMARK_TEMPLATE(serialize_contvec_bond, JsonWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_contvec_bond, JsonWriter, JsonReader, 10000);

BENCHMARK_TEMPLATE(serialize_contmap_bond, FastWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_contmap_bond, FastWriter, FastReader, 1000);
BENCHMARK_TEMPLATE(serialize_contmap_bond, FastWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_contmap_bond, FastWriter, FastReader, 10000);

BENCHMARK_TEMPLATE(serialize_contmap_bond, CompactWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_contmap_bond, CompactWriter, CompactReader, 1000);
BENCHMARK_TEMPLATE(serialize_contmap_bond, CompactWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_contmap_bond, CompactWriter, CompactReader, 10000);

BENCHMARK_TEMPLATE(serialize_contmap_bond, JsonWriter, 1000);
BENCHMARK_TEMPLATE(deserialize_contmap_bond, JsonWriter, JsonReader, 1000);
BENCHMARK_TEMPLATE(serialize_contmap_bond, JsonWriter, 10000);
BENCHMARK_TEMPLATE(deserialize_contmap_bond, JsonWriter, JsonReader, 10000);

int main(int argc, char** argv)
{
   setup();
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}
