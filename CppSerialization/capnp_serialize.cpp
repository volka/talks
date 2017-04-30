#include "benchmark/benchmark_api.h"
#include <functional>
#include <iostream>
#include <vector>

#include <capnp/message.h>
#include <capnp/serialize-packed.h>

#include "capnp/serialize.capnp.h"
#include "datasets.h"

using writerFunc = std::function<void(kj::OutputStream&, capnp::MessageBuilder&)>;
using generatorFunc = std::function<size_t(kj::OutputStream&, writerFunc, size_t)>;

constexpr static int MAX_BUF_SIZE = 8 * 1024 * 1024;
static std::vector<kj::byte> global_buffer;
static std::vector<kj::byte> global_scratch_buffer;

static void setup()
{
   global_buffer.resize(MAX_BUF_SIZE);
   global_scratch_buffer.resize(MAX_BUF_SIZE);
}

size_t generateInts(int int_val, kj::OutputStream& out, writerFunc writer, size_t count)
{
   size_t generated = 0;
   capnp::MallocMessageBuilder builder;

   auto msg = builder.initRoot<capnp_bench::IntList>();
   auto int_builder = msg.initInts(static_cast<unsigned int>(count));
   for (int i = 0; i < static_cast<int>(count); ++i)
   {
      auto current = int_builder[static_cast<unsigned int>(i)];
      current.setId(i);
      current.setInt1(int_val++);
      current.setInt2(int_val + 1);
      current.setInt3(int_val + i);
      current.setInt4(int_val + i + 40);
   }
   generated += capnp::computeSerializedSizeInWords(builder) * sizeof(capnp::word);
   writer(out, builder);
   return generated;
}
size_t generateMixed(kj::OutputStream& out, writerFunc writer, size_t count)
{
   size_t generated = 0;
   capnp::MallocMessageBuilder builder;
   int int_val = 123456;

   auto msg = builder.initRoot<capnp_bench::MixedList>();
   auto mixed_builder = msg.initMixed(static_cast<unsigned int>(count));
   for (int i = 0; i < static_cast<int>(count); ++i)
   {
      auto current = mixed_builder[static_cast<unsigned int>(i)];
      current.setId(i);
      current.setInt1(int_val + 10);
      current.setInt2(int_val++);
      current.setUint1(2);
      current.setFloat1(3.14);
      current.setEnum1((i % 2 == 0) ? ::capnp_bench::ENUMERATION::ONE
                                    : ::capnp_bench::ENUMERATION::TWO);
      current.setText1(string_short);
   }
   generated += capnp::computeSerializedSizeInWords(builder) * sizeof(capnp::word);
   writer(out, builder);
   return generated;
}

size_t generateComplex(kj::OutputStream& out, writerFunc writer, size_t count)
{
   size_t generated = 0;
   capnp::MallocMessageBuilder builder;
   int int_val = 123456;

   auto msg = builder.initRoot<capnp_bench::ComplexList>();
   auto complex_builder = msg.initComplex(static_cast<unsigned int>(count));
   for (int i = 0; i < static_cast<int>(count); ++i)
   {
      auto current = complex_builder[static_cast<unsigned int>(i)];
      current.setId(i);
      current.setInt1(int_val * int_val);
      current.setInt2(int_val * 2);
      current.setInt3(int_val);
      current.setInt4(int_val % 100);
      current.setInt5(-int_val);
      current.setInt6(int_val++);
      current.setEnum1((i % 2 == 0) ? ::capnp_bench::ENUMERATION::ONE
                                    : ::capnp_bench::ENUMERATION::TWO);
      current.setFloat1(3.14 * i);
      current.setFloat2(3.14 * int_val);
      current.setUint1(static_cast<uint32_t>(int_val));
      current.setUint2(static_cast<uint64_t>(int_val * int_val));
      current.setText1(string_short + std::to_string(i));
      current.setText2(string_short + string_short);
      current.setText3(string_long);
      current.setText4(string_short);
      current.setText5(string_long + std::to_string(int_val));
   }
   generated += capnp::computeSerializedSizeInWords(builder) * sizeof(capnp::word);
   writer(out, builder);
   return generated;
}

size_t generateContVec(kj::OutputStream& out, writerFunc writer, size_t count)
{
   size_t generated = 0;
   capnp::MallocMessageBuilder builder;
   int int_val = 123456;
   uint cont_count = 50;

   auto msg = builder.initRoot<capnp_bench::ContainerVecList>();
   auto cont_builder = msg.initContvec(static_cast<unsigned int>(count));
   for (int i = 0; i < static_cast<int>(count); ++i)
   {
      auto current = cont_builder[static_cast<unsigned int>(i)];
      current.setId(i);
      current.setInt1(int_val++);
      current.setText1(string_short);
      auto stringvec = current.initStringvec(cont_count);
      auto intvec = current.initIntvec(cont_count);
      for (uint j = 0; j < cont_count; ++j)
      {
         stringvec.set(j, string_short);
         intvec.set(j, int_val);
      }
   }
   generated += capnp::computeSerializedSizeInWords(builder) * sizeof(capnp::word);
   writer(out, builder);
   return generated;
}

size_t generateContMap(kj::OutputStream& out, writerFunc writer, size_t count)
{
   size_t generated = 0;
   capnp::MallocMessageBuilder builder;
   int int_val = 123456;
   uint cont_count = 20;

   auto msg = builder.initRoot<capnp_bench::ContainerMapList>();
   auto cont_builder = msg.initContmap(static_cast<unsigned int>(count));
   for (int i = 0; i < static_cast<int>(count); ++i)
   {
      auto current = cont_builder[static_cast<unsigned int>(i)];
      current.setId(i);
      current.setInt1(int_val++);
      current.setText1(string_short);
      current.initMap1();
      auto mapb = current.getMap1();
      auto stringmap = mapb.initById();
      auto entries = stringmap.initEntries(cont_count);
      for (uint j = 0; j < cont_count; ++j)
      {
         entries[j].initKey().setId(static_cast<int>(j));
         entries[j].setValue(string_short);
      }
   }
   generated += capnp::computeSerializedSizeInWords(builder) * sizeof(capnp::word);
   writer(out, builder);
   return generated;
}

// generic reader -> setRoot will copy the messages and validate the structure
template <class Reader>
void readMessage(capnp::MessageBuilder& out, Reader& in)
{
   out.setRoot(in.template getRoot<capnp::AnyPointer>());
}

void runSerialize(benchmark::State& st, size_t count, generatorFunc gen, writerFunc writer)
{
   size_t generated = 0;
   kj::ArrayPtr<kj::byte> arr(global_buffer.data(), global_buffer.size());
   while (st.KeepRunning())
   {
      kj::ArrayOutputStream out(arr);

      gen(out, writer, count);
      generated += out.getArray().size();
   }
   st.SetBytesProcessed(generated);
   st.SetItemsProcessed(st.iterations() * count);
   printSize(st, generated, static_cast<int>(count));
}

// ReaderType is the Message Reader (e.g. Packed/Normal), MsgType the *target* class from the
// datasets.h
// header
// the generatorFunc is one of the generate(Ints,Mixed,...) functions, while the writer is the
// function
// for writing Messages matching the ReaderType (see getWriter())
template <class ReaderType>
void runDeSerialize(benchmark::State& st, size_t count, generatorFunc gen, writerFunc writer)
{
   capnp::MallocMessageBuilder builder;

   kj::ArrayPtr<kj::byte> arr(global_buffer.data(), global_buffer.size());
   // kj::ArrayPtr<capnp::word>
   // scratch_arr(reinterpret_cast<capnp::word*>(global_scratch_buffer.data()),
   //                                      global_scratch_buffer.size()/sizeof(capnp::word));
   kj::ArrayOutputStream out(arr);
   gen(out, writer, count);
   size_t bytes = out.getArray().size();

   capnp::ReaderOptions readerOpts;
   readerOpts.traversalLimitInWords = MAX_BUF_SIZE * 8;

   while (st.KeepRunning())
   {
      kj::ArrayInputStream in(arr);
      ReaderType messages(in, readerOpts);

      readMessage(builder, messages);
   }
   st.SetBytesProcessed(bytes * st.iterations());
   st.SetItemsProcessed(st.iterations() * count);
   printSize(st, bytes * st.iterations(), static_cast<int>(count));
}

enum class SerializationType
{
   Default,
   Packed
};

using WriteMsg_t = void (*)(kj::OutputStream&, capnp::MessageBuilder&);

static writerFunc getWriter(const SerializationType type)
{
   switch (type)
   {
   case SerializationType::Default:
      return static_cast<WriteMsg_t>(capnp::writeMessage);
   case SerializationType::Packed:
      return static_cast<WriteMsg_t>(capnp::writePackedMessage);
   }
   return static_cast<WriteMsg_t>(capnp::writeMessage);
}

// some MPL to get the reader type form the SerializationType
template <SerializationType type>
struct Reader
{
};
template <>
struct Reader<SerializationType::Default>
{
   using type = capnp::InputStreamMessageReader;
};
template <>
struct Reader<SerializationType::Packed>
{
   using type = capnp::PackedMessageReader;
};

template <size_t COUNT, int INT_VAL, SerializationType TYPE>
void serialize_ints_capnp(benchmark::State& st)
{
   runSerialize(st, COUNT, std::bind(generateInts, INT_VAL, std::placeholders::_1,
                                     std::placeholders::_2, std::placeholders::_3),
                getWriter(TYPE));
}

template <size_t COUNT, int INT_VAL, SerializationType TYPE>
void deserialize_ints_capnp(benchmark::State& st)
{
   runDeSerialize<typename Reader<TYPE>::type>(
       st, COUNT, std::bind(generateInts, INT_VAL, std::placeholders::_1, std::placeholders::_2,
                            std::placeholders::_3),
       getWriter(TYPE));
}

template <size_t COUNT, SerializationType TYPE>
void serialize_mixed_capnp(benchmark::State& st)
{
   runSerialize(st, COUNT, generateMixed, getWriter(TYPE));
}

template <size_t COUNT, SerializationType TYPE>
void deserialize_mixed_capnp(benchmark::State& st)
{
   runDeSerialize<typename Reader<TYPE>::type>(st, COUNT, generateMixed, getWriter(TYPE));
}

template <size_t COUNT, SerializationType TYPE>
void serialize_complex_capnp(benchmark::State& st)
{
   runSerialize(st, COUNT, generateComplex, getWriter(TYPE));
}

template <size_t COUNT, SerializationType TYPE>
void deserialize_complex_capnp(benchmark::State& st)
{
   runDeSerialize<typename Reader<TYPE>::type>(st, COUNT, generateComplex, getWriter(TYPE));
}

template <size_t COUNT, SerializationType TYPE>
void serialize_contvec_capnp(benchmark::State& st)
{
   runSerialize(st, COUNT, generateContVec, getWriter(TYPE));
}

template <size_t COUNT, SerializationType TYPE>
void deserialize_contvec_capnp(benchmark::State& st)
{
   runDeSerialize<typename Reader<TYPE>::type>(st, COUNT, generateContVec, getWriter(TYPE));
}

template <size_t COUNT, SerializationType TYPE>
void serialize_contmap_capnp(benchmark::State& st)
{
   runSerialize(st, COUNT, generateContMap, getWriter(TYPE));
}

template <size_t COUNT, SerializationType TYPE>
void deserialize_contmap_capnp(benchmark::State& st)
{
   runDeSerialize<typename Reader<TYPE>::type>(st, COUNT, generateContMap, getWriter(TYPE));
}

BENCHMARK_TEMPLATE(serialize_ints_capnp, 1000, small_int, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_ints_capnp, 10000, small_int, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_ints_capnp, 1000, small_int, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_ints_capnp, 10000, small_int, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_ints_capnp, 1000, small_int, SerializationType::Packed);
BENCHMARK_TEMPLATE(serialize_ints_capnp, 10000, small_int, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_ints_capnp, 1000, small_int, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_ints_capnp, 10000, small_int, SerializationType::Packed);
BENCHMARK_TEMPLATE(serialize_ints_capnp, 1000, integer, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_ints_capnp, 10000, integer, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_ints_capnp, 1000, integer, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_ints_capnp, 10000, integer, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_ints_capnp, 1000, integer, SerializationType::Packed);
BENCHMARK_TEMPLATE(serialize_ints_capnp, 10000, integer, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_ints_capnp, 1000, integer, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_ints_capnp, 10000, integer, SerializationType::Packed);

BENCHMARK_TEMPLATE(serialize_mixed_capnp, 1000, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_mixed_capnp, 10000, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_mixed_capnp, 1000, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_mixed_capnp, 10000, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_mixed_capnp, 1000, SerializationType::Packed);
BENCHMARK_TEMPLATE(serialize_mixed_capnp, 10000, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_mixed_capnp, 1000, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_mixed_capnp, 10000, SerializationType::Packed);

BENCHMARK_TEMPLATE(serialize_complex_capnp, 1000, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_complex_capnp, 10000, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_complex_capnp, 1000, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_complex_capnp, 10000, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_complex_capnp, 1000, SerializationType::Packed);
BENCHMARK_TEMPLATE(serialize_complex_capnp, 10000, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_complex_capnp, 1000, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_complex_capnp, 10000, SerializationType::Packed);

BENCHMARK_TEMPLATE(serialize_contvec_capnp, 1000, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_contvec_capnp, 10000, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_contvec_capnp, 1000, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_contvec_capnp, 10000, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_contvec_capnp, 1000, SerializationType::Packed);
BENCHMARK_TEMPLATE(serialize_contvec_capnp, 10000, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_contvec_capnp, 1000, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_contvec_capnp, 10000, SerializationType::Packed);
BENCHMARK_TEMPLATE(serialize_contvec_capnp, 1000, SerializationType::Default);

BENCHMARK_TEMPLATE(serialize_contmap_capnp, 1000, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_contmap_capnp, 10000, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_contmap_capnp, 1000, SerializationType::Default);
BENCHMARK_TEMPLATE(deserialize_contmap_capnp, 10000, SerializationType::Default);
BENCHMARK_TEMPLATE(serialize_contmap_capnp, 1000, SerializationType::Packed);
BENCHMARK_TEMPLATE(serialize_contmap_capnp, 10000, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_contmap_capnp, 1000, SerializationType::Packed);
BENCHMARK_TEMPLATE(deserialize_contmap_capnp, 10000, SerializationType::Packed);

int main(int argc, char** argv)
{
   setup();
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}
