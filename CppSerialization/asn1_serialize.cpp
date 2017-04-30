#include <functional>
#include <iostream>

#include "asn1/dx_berenc.h"

#include "datasets.h"

#include "benchmark/benchmark_api.h"

/// Hackaty hack hack ...
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

using generatorFunc = std::function<void(BerEnc*, size_t)>;
template<class TYPE>
using readerFunc = std::function<void(BerCodec_t*, TYPE&)>;

constexpr static int MAX_BUF_SIZE = 40 * 1024 * 1024;
static std::vector<char> global_buffer;
static std::vector<char> global_buffer_out;

void setupConfiguration() {
   global_buffer.resize(MAX_BUF_SIZE);
   global_buffer_out.resize(MAX_BUF_SIZE);
}

template <int INT_VAL>
void generateInts(BerEnc* cd, size_t count)
{
   int int_val = INT_VAL;
   cd->BerBegin(BER_CONSTRUCTED | BER_SEQUENCE);
   for (size_t i = 0; i < count; ++i)
   {
      int_val += i;
      int i_int = static_cast<int>(i);
      cd->BerBegin(BER_CONSTRUCTED + BER_CONTEXT_SPECIFIC + 1);

      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 1, i_int);
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 2, int_val++);
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 3, int_val + 1);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 4, int_val + i_int);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 5, int_val + i_int + 40);
      cd->BerEnd();
   }
   cd->BerEnd();
}

void readInts(BerCodec_t* cd, ints_t& current)
{
   int ec = 0;
   ec = BerEnter(cd);
   // don't switch on tags, just assume correct layout
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.id));
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.int1));
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.int2));
   BerNext(cd);
   if (!ec) ec = BerGetInt(cd, &current.int3);
   BerNext(cd);
   if (!ec) ec = BerGetInt(cd, &current.int4);
   if (!ec) ec = BerLeave(cd);
   if (!ec) ec = BerNext(cd);
   if (ec) throw std::runtime_error("Error decoding ints_t value 8");
}

void generateMixed(BerEnc* cd, size_t count)
{
   int int_val = 123456;
   static double pi = 3.14;
   cd->BerBegin(BER_CONSTRUCTED | BER_SEQUENCE);
   for (size_t i = 0; i < count; ++i)
   {
      int_val += i;
      int i_int = static_cast<int>(i);

      cd->BerBegin(BER_CONSTRUCTED + BER_CONTEXT_SPECIFIC + 1);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 1, i_int);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 2, int_val + 10);
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 3, int_val++);
      cd->BerPutUnsigned(BER_CONTEXT_SPECIFIC + 4,
                     static_cast<unsigned>(int_val + 1));
      // non-portable HACK, we don't support float values
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 5, *reinterpret_cast<long long*>(&pi));
      cd->BerPutStdString(BER_CONTEXT_SPECIFIC + 6, string_short);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 6, static_cast<int>(
                   (i % 2 == 0) ? ENUMERATION::ONE : ENUMERATION::TWO));
      cd->BerEnd();
   }
   cd->BerEnd();
}

void readMixed(BerCodec_t* cd, mixed_t& current)
{
   int ec = 0;
   ec = BerEnter(cd);
   // don't switch on tags, just assume correct layout
   if (!ec) ec = BerGetInt(cd, &current.id);
   BerNext(cd);
   if (!ec) ec = BerGetInt(cd, &current.int1);
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.int2));
   BerNext(cd);
   if (!ec) ec = BerGetUnsigned(cd, reinterpret_cast<unsigned*>(&current.uint1));
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.float1));
   BerNext(cd);
   if (!ec) ec = BerGetStdString(cd, current.text1);
   BerNext(cd);
   if (!ec) ec = BerGetInt(cd, reinterpret_cast<int*>(&current.enum1));
   if (!ec) ec = BerLeave(cd);
   BerNext(cd);
   if (ec) throw std::runtime_error("Error decoding mixed_t value");
}

void generateContainerVec(BerEnc* cd, size_t count)
{
   int int_val = 123456;
   size_t cont_count = 50;

   cd->BerBegin(BER_CONSTRUCTED | BER_SEQUENCE);
   for (size_t i = 0; i < count; ++i)
   {
      int_val += i;
      int i_int = static_cast<int>(i);

      cd->BerBegin(BER_CONSTRUCTED + BER_CONTEXT_SPECIFIC + 1);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 1, i_int);
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 2, int_val++);
      cd->BerPutStdString(BER_CONTEXT_SPECIFIC + 3, string_short);

      // stringvec
      cd->BerBegin((BER_CONSTRUCTED | BER_SEQUENCE) + 4);
      for (size_t j = 0; j < cont_count; ++j) {
         cd->BerPutStdString(BER_CONTEXT_SPECIFIC + j, string_short);
      }
      cd->BerEnd();
      // intvec
      cd->BerBegin((BER_CONSTRUCTED | BER_SEQUENCE) + 5);
      for (size_t j = 0; j < cont_count; ++j) {
         cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + j, int_val);
      }
      cd->BerEnd();

      cd->BerEnd();
   }
   cd->BerEnd();
}

void readContVec(BerCodec_t* cd, container_vec_t& current)
{
   size_t cont_count = 50;
   int ec = 0;
   ec = BerEnter(cd);
   // don't switch on tags, just assume correct layout
   if (!ec) ec = BerGetInt(cd, &current.id);
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.int1));
   BerNext(cd);
   if (!ec) ec = BerGetStdString(cd, current.text1);
   BerNext(cd);
   BerEnter(cd);
   current.stringvec.clear();
   current.stringvec.resize(cont_count);
   for (size_t i = 0; i < cont_count; ++i) {
      if (!ec) ec = BerGetStdString(cd, current.stringvec[i]);
   }
   BerLeave(cd);
   BerNext(cd);
   BerEnter(cd);
   current.intvec.clear();
   current.intvec.resize(cont_count);
   for (size_t i = 0; i < cont_count; ++i) {
      if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.intvec[i]));
   }
   if (!ec) ec = BerLeave(cd);
   if (!ec) ec = BerLeave(cd);
   if (!ec) ec = BerNext(cd);
   if (ec) throw std::runtime_error("Error decoding cont_vec value");
}

void generateContainerMap(BerEnc* cd, size_t count)
{
   int int_val = 123456;
   int cont_count = 20;

   cd->BerBegin(BER_CONSTRUCTED | BER_SEQUENCE);
   for (size_t i = 0; i < count; ++i)
   {
      int_val += i;
      int i_int = static_cast<int>(i);

      cd->BerBegin(BER_CONSTRUCTED + BER_CONTEXT_SPECIFIC + 1);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 1, i_int);
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 2, int_val++);
      cd->BerPutStdString(BER_CONTEXT_SPECIFIC + 3, string_short);

      // map [int -> string]
      cd->BerBegin((BER_CONSTRUCTED | BER_SEQUENCE) + 4);
      for (int j = 0; j < cont_count; ++j) {
         cd->BerPutInt(BER_CONTEXT_SPECIFIC + (2*j), j);
         cd->BerPutStdString(BER_CONTEXT_SPECIFIC + (2*j+1), string_short);
      }
      cd->BerEnd();

      cd->BerEnd();
   }
   cd->BerEnd();
}

void readContMap(BerCodec_t* cd, container_map_t& current)
{
   size_t cont_count = 50;
   int ec = 0;
   ec = BerEnter(cd);
   // don't switch on tags, just assume correct layout
   if (!ec) ec = BerGetInt(cd, &current.id);
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.int1));
   BerNext(cd);
   if (!ec) ec = BerGetStdString(cd, current.text1);
   BerNext(cd);
   BerEnter(cd);
   current.map1.clear();
   int idx = 0;
   std::string tmp;
   decltype(current.map1)::iterator it = std::begin(current.map1);
   for (size_t i = 0; i < cont_count; ++i) {
      if (!ec) ec = BerGetInt(cd, &idx);
      if (!ec) ec = BerGetStdString(cd, tmp);
      if (!ec) it = current.map1.emplace_hint(it, idx, std::move(tmp));
   }
   if (!ec) ec = BerLeave(cd);
   if (!ec) ec = BerLeave(cd);
   if (!ec) ec = BerNext(cd);
   if (ec) throw std::runtime_error("Error decoding cont_map value");
}

void generateComplex(BerEnc* cd, size_t count)
{
   int int_val = 123456;
   static double pi = 3.14;
   double pi_2;
   cd->BerBegin(BER_CONSTRUCTED | BER_SEQUENCE);
   for (size_t i = 0; i < count; ++i)
   {
      int_val += i;
      int i_int = static_cast<int>(i);

      cd->BerBegin(BER_CONSTRUCTED + BER_CONTEXT_SPECIFIC + 1);
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 1, i_int);
      cd->BerPutStdString(BER_CONTEXT_SPECIFIC + 2, string_short + std::to_string(i));
      cd->BerPutStdString(BER_CONTEXT_SPECIFIC + 3, string_short + string_short);
      cd->BerPutStdString(BER_CONTEXT_SPECIFIC + 4, string_long);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 5, int_val * int_val);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 6, int_val * 2);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 7, int_val);
      cd->BerPutStdString(BER_CONTEXT_SPECIFIC + 8, string_long);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 9, static_cast<int>(
                    (i % 2 == 0) ? ENUMERATION::ONE : ENUMERATION::TWO));
      pi_2 = pi * i;
      // non-portable HACK, we don't support float values
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 10, *reinterpret_cast<long long*>(&pi_2));

      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 11, int_val % 100);

      // uint1 - BerEnc does not support this, use signed instead
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 12,
                      int_val);
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 13,
                      static_cast<long long>(int_val * int_val));
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 14, -int_val);
      cd->BerPutInt(BER_CONTEXT_SPECIFIC + 15, int_val++);
      pi_2 = pi * int_val;
      // non-portable HACK, we don't support float values
      cd->BerPutLongLong(BER_CONTEXT_SPECIFIC + 5, *reinterpret_cast<long long*>(&pi_2));
      cd->BerPutStdString(BER_CONTEXT_SPECIFIC + 6, string_long + std::to_string(int_val));
      cd->BerEnd();
   }
   cd->BerEnd();
}

void readComplex(BerCodec_t* cd, complex_t& current)
{
   int ec = 0;
   ec = BerEnter(cd);
   // don't switch on tags, just assume correct layout
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.id));
   BerNext(cd);
   if (!ec) ec = BerGetStdString(cd, current.text1);
   BerNext(cd);
   if (!ec) ec = BerGetStdString(cd, current.text2);
   BerNext(cd);
   if (!ec) ec = BerGetStdString(cd, current.text3);
   BerNext(cd);
   if (!ec) ec = BerGetInt(cd, &current.int1);
   BerNext(cd);
   if (!ec) ec = BerGetInt(cd, &current.int2);
   BerNext(cd);
   if (!ec) ec = BerGetInt(cd, &current.int3);
   BerNext(cd);
   if (!ec) ec = BerGetStdString(cd, current.text4);
   BerNext(cd);
   if (!ec) ec = BerGetInt(cd, reinterpret_cast<int*>(&current.enum1));
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.float1));
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.int4));
   BerNext(cd);
   if (!ec) ec = BerGetInt(cd, reinterpret_cast<int*>(&current.uint1));
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.uint2));
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.int5));
   BerNext(cd);
   if (!ec) ec = BerGetInt(cd, &current.int6);
   BerNext(cd);
   if (!ec) ec = BerGetLongLong(cd, reinterpret_cast<long long*>(&current.float2));
   BerNext(cd);
   if (!ec) ec = BerGetStdString(cd, current.text5);
   BerNext(cd);
   if (!ec) ec = BerLeave(cd);
   BerNext(cd);
}

// generic serialize function
void runSerialize(benchmark::State& state, size_t count, generatorFunc& generator)
{
   size_t generated = 0;
   BerEnc codec(global_buffer.data(), static_cast<int>(global_buffer.size()));
   int size = 0;

   while (state.KeepRunning())
   {
      codec.Clear();

      generator(&codec, count);

      codec.BerResultSize(&size);
      generated += static_cast<size_t>(size);
   }
   state.SetBytesProcessed(generated);
   state.SetItemsProcessed(state.iterations() * count);

   printSize(state, generated, static_cast<int>(count));
}

// generic deserialize test function, relatively huge overhead through generation may skew results
template <class TYPE>
void runDeSerialize(benchmark::State& state, size_t count, generatorFunc& generator,
                    readerFunc<TYPE>& reader)
{
   size_t bytes = 0;
   std::vector<TYPE> data(count);

   BerEnc in_codec(global_buffer.data(),
                   static_cast<int>(global_buffer.size()));

   generator(&in_codec, count);
   int len = 0;
   in_codec.BerResultSize(&len);
   bytes = static_cast<size_t>(len);

   while (state.KeepRunning())
   {
      BerCodec_t* out_codec = BerAlloc(global_buffer.data(), len);
      for (auto& current : data)
      {
         BerEnter(out_codec);
         reader(out_codec, current);
         BerLeave(out_codec);
      }
      BerFree(out_codec);
   }
   state.SetBytesProcessed(state.iterations() * bytes);
   state.SetItemsProcessed(state.iterations() * count);
   printSize(state, state.iterations() * bytes, static_cast<int>(count));
}

// type specific benchmark entry points
template <int COUNT>
void serialize_ints_asn1(benchmark::State& state)
{
   generatorFunc gen = generateInts<small_int>;
   runSerialize(state, COUNT, gen);
}

template <int COUNT>
void deserialize_ints_asn1(benchmark::State& state)
{
   generatorFunc gen = generateInts<small_int>;
   readerFunc<ints_t> reader = readInts;
   runDeSerialize<ints_t>(state, COUNT, gen, reader);
}

template <int COUNT>
void serialize_large_ints_asn1(benchmark::State& state)
{
   generatorFunc gen = generateInts<integer>;
   runSerialize(state, COUNT, gen);
}

template <int COUNT>
void deserialize_large_ints_asn1(benchmark::State& state)
{
   generatorFunc gen = generateInts<integer>;
   readerFunc<ints_t> reader = readInts;
   runDeSerialize<ints_t>(state, COUNT, gen, reader);
}

template <int COUNT>
void serialize_mixed_asn1(benchmark::State& state)
{
   generatorFunc gen = generateMixed;
   runSerialize(state, COUNT, gen);
}

template <int COUNT>
void deserialize_mixed_asn1(benchmark::State& state)
{
   generatorFunc gen = generateMixed;
   readerFunc<mixed_t> reader = readMixed;
   runDeSerialize<mixed_t>(state, COUNT, gen, reader);
}

template <int COUNT>
void serialize_complex_asn1(benchmark::State& state)
{
   generatorFunc gen = generateComplex;
   runSerialize(state, COUNT, gen);
}

template <int COUNT>
void deserialize_complex_asn1(benchmark::State& state)
{
   generatorFunc gen = generateComplex;
   readerFunc<complex_t> reader = readComplex;
   runDeSerialize<complex_t>(state, COUNT, gen, reader);
}

template <int COUNT>
void serialize_contvec_asn1(benchmark::State& state)
{
   generatorFunc gen = generateContainerVec;
   runSerialize(state, COUNT, gen);
}

template <int COUNT>
void deserialize_contvec_asn1(benchmark::State& state)
{
   generatorFunc gen = generateContainerVec;
   readerFunc<container_vec_t> reader = readContVec;
   runDeSerialize<container_vec_t>(state, COUNT, gen, reader);
}

template <int COUNT>
void serialize_contmap_asn1(benchmark::State& state)
{
   generatorFunc gen = generateContainerMap;
   runSerialize(state, COUNT, gen);
}

template <int COUNT>
void deserialize_contmap_asn1(benchmark::State& state)
{
   generatorFunc gen = generateContainerMap;
   readerFunc<container_map_t> reader = readContMap;
   runDeSerialize<container_map_t>(state, COUNT, gen, reader);
}

BENCHMARK_TEMPLATE(serialize_ints_asn1, 1000);
BENCHMARK_TEMPLATE(serialize_ints_asn1, 10000);

BENCHMARK_TEMPLATE(deserialize_ints_asn1, 1000);
BENCHMARK_TEMPLATE(deserialize_ints_asn1, 10000);

BENCHMARK_TEMPLATE(serialize_large_ints_asn1, 1000);
BENCHMARK_TEMPLATE(serialize_large_ints_asn1, 10000);

BENCHMARK_TEMPLATE(deserialize_large_ints_asn1, 1000);
BENCHMARK_TEMPLATE(deserialize_large_ints_asn1, 10000);

BENCHMARK_TEMPLATE(serialize_mixed_asn1, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_asn1, 10000);

BENCHMARK_TEMPLATE(deserialize_mixed_asn1, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_asn1, 10000);

BENCHMARK_TEMPLATE(serialize_complex_asn1, 1000);
BENCHMARK_TEMPLATE(serialize_complex_asn1, 10000);

BENCHMARK_TEMPLATE(deserialize_complex_asn1, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_asn1, 10000);

BENCHMARK_TEMPLATE(serialize_contvec_asn1, 1000);
BENCHMARK_TEMPLATE(serialize_contvec_asn1, 10000);

BENCHMARK_TEMPLATE(deserialize_contvec_asn1, 1000);
BENCHMARK_TEMPLATE(deserialize_contvec_asn1, 10000);

BENCHMARK_TEMPLATE(serialize_contmap_asn1, 1000);
BENCHMARK_TEMPLATE(serialize_contmap_asn1, 10000);

BENCHMARK_TEMPLATE(deserialize_contmap_asn1, 1000);
BENCHMARK_TEMPLATE(deserialize_contmap_asn1, 10000);

int main(int argc, char** argv)
{
   setupConfiguration();
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}
