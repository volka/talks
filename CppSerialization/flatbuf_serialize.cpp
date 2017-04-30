#include <functional>

#include "benchmark/benchmark_api.h"

#include <zlib.h>

#include "datasets.h"
#include "flatbuf/serialize_generated.h"
#include <flatbuffers/flatbuffers.h>

using generatorFunc = std::function<void(flatbuffers::FlatBufferBuilder&, size_t)>;
using readerFunc = std::function<bool(const flatbuf_bench::Root* root)>;

constexpr static int MAX_BUF_SIZE = 40 * 1024 * 1024;
static std::vector<unsigned char> global_buffer;
static std::vector<unsigned char> compress_buffer;
static std::unique_ptr<flatbuffers::FlatBufferBuilder> global_builder_ptr;

static void setup()
{
   global_buffer.resize(MAX_BUF_SIZE);
   compress_buffer.resize(MAX_BUF_SIZE);
   global_builder_ptr.reset(new flatbuffers::FlatBufferBuilder(MAX_BUF_SIZE));
}

// clear the global buffer and hand out a reference
static flatbuffers::FlatBufferBuilder& getBuilder()
{
   if (!global_builder_ptr)
      throw std::runtime_error("Global builder not initialized, run setup() first");
   global_builder_ptr->Clear();
   return *global_builder_ptr;
}

using namespace flatbuf_bench;

template <int INT_VAL>
void generateInts(flatbuffers::FlatBufferBuilder& builder, size_t count)
{
   using namespace flatbuffers;
   int int_val = INT_VAL;
   int int_val_p1 = int_val + 1;
   int i_int = 0;

   if (sizeof(Ints)*count > global_buffer.size())
      throw std::runtime_error("Statically allocated global buffer too small for Ints test");
   auto intvec = reinterpret_cast<Ints*>(global_buffer.data());

   for (size_t i = 0; i < count; ++i)
   {
      i_int = static_cast<int>(i);
      intvec[i] = Ints(i_int, int_val, int_val_p1 + 1, int_val + i_int, int_val + i_int + 40);
   }

   auto intvec_buf = builder.CreateVectorOfStructs(intvec, count);
   auto root = CreateRoot(builder, intvec_buf);

   FinishRootBuffer(builder, root);
}

void generateMixed(flatbuffers::FlatBufferBuilder& builder, size_t count)
{
   int int_val = 123456;
   if (sizeof(flatbuffers::Offset<Mixed>)*count > global_buffer.size())
      throw std::runtime_error("Statically allocated global buffer too small for Mixed offsets");

   auto mixbuf = reinterpret_cast<flatbuffers::Offset<Mixed>*>(global_buffer.data());

   for (int i = 0; i < static_cast<int>(count); ++i)
   {
      auto text1 = builder.CreateString(string_short.c_str(), string_short.size());
      MixedBuilder current(builder);
      current.add_id(i);
      current.add_int1(int_val + 10);
      current.add_int2(int_val++);
      current.add_uint1(2);
      current.add_float1(3.14);
      current.add_text1(text1);
      current.add_enum1( (i % 2 == 0) ? ENUMERATION_ONE : ENUMERATION_TWO);
      mixbuf[static_cast<size_t>(i)] = current.Finish();
   }
   auto mixed_vec = builder.CreateVector(mixbuf, count);
   auto root = CreateRoot(builder, 0, mixed_vec);
   FinishRootBuffer(builder, root);
}

void generateContainerVec(flatbuffers::FlatBufferBuilder& builder, size_t count)
{
   int int_val = 123456;
   int cont_count = 50;

   if (sizeof(flatbuffers::Offset<ContainerVec>)*count > global_buffer.size())
      throw std::runtime_error("Statically allocated global buffer too small for Mixed offsets");

   auto vecbuf = reinterpret_cast<flatbuffers::Offset<ContainerVec>*>(global_buffer.data());

   std::vector<flatbuffers::Offset<flatbuffers::String>> stringvec;
   std::vector<int64_t> intvec;

   for (size_t i = 0; i < count; ++i)
   {
      ContainerVecBuilder current(builder);
      stringvec.clear(); // reuse buffers
      intvec.clear();

      current.add_id(static_cast<int>(i));
      current.add_int1(int_val++);
      auto text1 = builder.CreateString(string_short.c_str(), string_short.size());
      current.add_text1(text1);

      for (int j = 0; j < cont_count; ++j)
      {
         stringvec.push_back(builder.CreateString(string_short.c_str(), string_short.size()));
         intvec.push_back(int_val);
      }
      auto stringvec_flat = builder.CreateVector(stringvec);
      auto intvec_flat = builder.CreateVector(intvec);
      current.add_intvec(intvec_flat);
      current.add_stringvec(stringvec_flat);
      vecbuf[i] = current.Finish();
   }
   auto contvec_vec = builder.CreateVector(vecbuf, count);
   auto root = CreateRoot(builder,0, 0, 0, contvec_vec);
   FinishRootBuffer(builder, root);
}

void generateContainerMap(flatbuffers::FlatBufferBuilder& builder, size_t count)
{
   int int_val = 123456;
   int cont_count = 20;

   if (sizeof(flatbuffers::Offset<ContainerMap>)*count > global_buffer.size())
      throw std::runtime_error("Statically allocated global buffer too small for Mixed offsets");

   auto mapbuf = reinterpret_cast<flatbuffers::Offset<ContainerMap>*>(global_buffer.data());

   std::vector<flatbuffers::Offset<flatbuf_bench::IntStringMap>> mapData;
   for (size_t i = 0; i < count; ++i)
   {
      // DO NOT INTERLEAVE Builder instances !!! IntStringMapBuilder needs to be finished when
      // ContainerMapBuilder is created ...
      for (int j = 0; j < cont_count; ++j)
      {
         IntStringMapBuilder mapBuilder(builder);
         mapBuilder.add_id(j);
         auto value = builder.CreateString(string_short);
         mapBuilder.add_value(value);
         mapData.push_back(mapBuilder.Finish());
      }
      ContainerMapBuilder current(builder);
      mapData.clear(); // reuse buffer

      auto text1 = builder.CreateString(string_short);
      current.add_id(static_cast<int>(i));
      current.add_int1(int_val++);
      current.add_text1(text1);

      auto mapData_flat = builder.CreateVector(mapData);
      current.add_map1(mapData_flat);
      mapbuf[i] = current.Finish();
   }
   auto contmap_vec = builder.CreateVector(mapbuf, count);
   auto root = CreateRoot(builder, 0, 0, 0, 0, contmap_vec);
   FinishRootBuffer(builder, root);
}

void generateComplex(flatbuffers::FlatBufferBuilder& builder, size_t count)
{
   int int_val = 123456;

   if (sizeof(flatbuffers::Offset<Complex>)*count > global_buffer.size())
      throw std::runtime_error("Statically allocated global buffer too small for Mixed offsets");


   auto complexbuf = reinterpret_cast<flatbuffers::Offset<Complex>*>(global_buffer.data());
   for (int i = 0; i < static_cast<int>(count); ++i)
   {
      auto text1_str = string_short + std::to_string(i);
      auto text1 = builder.CreateString(text1_str.c_str(), text1_str.size());
      auto text2_str = string_short + string_short;
      auto text2 = builder.CreateString(text2_str.c_str(), text2_str.size());
      auto text3 = builder.CreateString(string_long);
      auto text4 = builder.CreateString(string_short);
      auto text5_str = string_long + std::to_string(int_val);
      auto text5 = builder.CreateString(text5_str.c_str(), text5_str.size());

      ComplexBuilder current(builder);
      current.add_id(i);
      current.add_int1(int_val * int_val);
      current.add_int2(int_val * 2);
      current.add_int3(int_val);
      current.add_int4(int_val % 100);
      current.add_int5(-int_val);
      current.add_int6(int_val++);
      current.add_enum1((i % 2 == 0) ? ENUMERATION_ONE : ENUMERATION_TWO);
      current.add_float1(3.14 * i);
      current.add_float2(3.14 * int_val);
      current.add_uint1(static_cast<uint32_t>(int_val));
      current.add_uint2(static_cast<uint64_t>(int_val * int_val));
      current.add_text1(text1);
      current.add_text2(text2);
      current.add_text3(text3);
      current.add_text4(text4);
      current.add_text5(text5);

      complexbuf[static_cast<size_t>(i)] = current.Finish();
   }
   auto complex_vec = builder.CreateVector(complexbuf, count);
   auto root = CreateRoot(builder, 0, 0, complex_vec);
   FinishRootBuffer(builder, root);
}

//// ZLIB helper functions

// compress the in buffer into the out buffer using the deflate algorithm
// this was used for an optimization test, but we should really just use compress/uncompress
size_t z_compress(uint8_t* in, size_t in_size, uint8_t* out, size_t out_size,
                  int level = Z_BEST_SPEED)
{
   // hack to speed up compression, do not use multi-threaded :)
   z_stream defstream;

   // limit size to 4GB
   defstream.avail_in = static_cast<unsigned int>(in_size);
   defstream.next_in = in;
   defstream.avail_out = static_cast<unsigned int>(out_size);
   defstream.next_out = out;

   defstream.zalloc = Z_NULL;
   defstream.zfree = Z_NULL;
   defstream.opaque = Z_NULL;
   deflateInit(&defstream, level);
   deflate(&defstream, Z_FINISH);
   deflateEnd(&defstream);
   return defstream.total_out;
}

// this was used for an optimization test, but we should really just use compress/uncompress
size_t z_uncompress(uint8_t* in, size_t in_size, uint8_t* out, size_t out_size)
{
   // hack to speed up compression, do not use multi-threaded :)
   z_stream infstream;

   // limit size to 4GB
   infstream.avail_in = static_cast<unsigned int>(in_size);
   infstream.next_in = in;
   infstream.avail_out = static_cast<unsigned int>(out_size);
   infstream.next_out = out;

   infstream.zalloc = Z_NULL;
   infstream.zfree = Z_NULL;
   infstream.opaque = Z_NULL;
   inflateInit(&infstream);
   inflate(&infstream, Z_NO_FLUSH);
   inflateEnd(&infstream);
   return infstream.total_out;
}

// generic serialize function
void runSerialize(benchmark::State& state, size_t count, generatorFunc& generator, bool compress)
{
   size_t generated = 0;

   flatbuffers::FlatBufferBuilder& builder = getBuilder();

   while (state.KeepRunning())
   {
      generator(builder, count);
      if (compress) {
         generated += z_compress(builder.GetBufferPointer(), builder.GetSize(), compress_buffer.data(),
                                 compress_buffer.size());
         builder.Clear();
      } else {
         generated += builder.GetSize();
         builder.Clear();
      }
   }
   state.SetBytesProcessed(generated);
   state.SetItemsProcessed(state.iterations() * count);

   printSize(state, generated, static_cast<int>(count));
}

// compress result buffer using zlib


// generic deserialize test function, relatively huge overhead through generation may skew results
void runDeSerialize(benchmark::State& state, size_t count, generatorFunc& generator, readerFunc& reader,
                    bool compress)
{
   size_t bytes = 0;
   size_t uncompressed_size = 0;

   flatbuffers::FlatBufferBuilder& builder = getBuilder();
   uint8_t* buf = nullptr;

   {
      generator(builder, count);
      uncompressed_size = builder.GetSize();
      if (compress) {
         bytes = z_compress(builder.GetBufferPointer(), builder.GetSize(), compress_buffer.data(),
                            compress_buffer.size());
         buf = compress_buffer.data();
      } else {
         bytes = uncompressed_size;
         buf = builder.GetBufferPointer();
      }
   }

   while (state.KeepRunning())
   {
      if (compress) {
         z_uncompress(compress_buffer.data(), bytes, global_buffer.data(), global_buffer.size());
         buf = global_buffer.data();
      }
      auto verifier = flatbuffers::Verifier(buf, uncompressed_size);
      if (VerifyRootBuffer(verifier)) {
         auto root = GetRoot(buf);
         if (!reader(root)) throw std::runtime_error("Reader function failed");
      } else {
         throw std::runtime_error("Error parsing Root Buffer");
      }
   }

   state.SetBytesProcessed(state.iterations() * bytes);
   state.SetItemsProcessed(state.iterations() * static_cast<size_t>(count));
   printSize(state, state.iterations()*bytes, static_cast<int>(count));
}

// type specific benchmark entry points
template <size_t COUNT, int INT_VAL, bool COMPRESS>
void serialize_ints_flatbuf(benchmark::State& state)
{
   generatorFunc gen = generateInts<INT_VAL>;
   runSerialize(state, COUNT, gen, COMPRESS);
}

template<size_t COUNT, int INT_VAL, bool COMPRESS>
void deserialize_ints_flatbuf(benchmark::State& state)
{
   generatorFunc gen = generateInts<INT_VAL>;
   readerFunc reader = [](const flatbuf_bench::Root* root) -> bool {
      long long int counter = 0;
      for (const Ints* int_ptr: *root->ints()) {
         benchmark::DoNotOptimize(counter += int_ptr->id());
      }
      return counter != 0;
   };
   runDeSerialize(state, COUNT, gen, reader, COMPRESS);
}

template <size_t COUNT, bool COMPRESS>
void serialize_mixed_flatbuf(benchmark::State& state)
{
   generatorFunc gen = generateMixed;
   runSerialize(state, COUNT, gen, COMPRESS);
}

template<size_t COUNT, bool COMPRESS>
void deserialize_mixed_flatbuf(benchmark::State& state)
{
   generatorFunc gen = generateMixed;
   readerFunc reader = [](const flatbuf_bench::Root* root) -> bool {
      long long int counter = 0;
      for (const Mixed* ptr: *root->mixed()) {
         benchmark::DoNotOptimize(counter += ptr->id());
      }
      return counter != 0;
   };
   runDeSerialize(state, COUNT, gen, reader, COMPRESS);
}

template <size_t COUNT, bool COMPRESS>
void serialize_complex_flatbuf(benchmark::State& state)
{
   generatorFunc gen = generateComplex;
   runSerialize(state, COUNT, gen, COMPRESS);
}

template<size_t COUNT, bool COMPRESS>
void deserialize_complex_flatbuf(benchmark::State& state)
{
   generatorFunc gen = generateComplex;
   readerFunc reader = [](const flatbuf_bench::Root* root) -> bool {
      long long int counter = 0;
      for (const Complex* ptr: *root->complex()) {
         benchmark::DoNotOptimize(counter += ptr->id());
      }
      return counter != 0;
   };
   runDeSerialize(state, COUNT, gen, reader, COMPRESS);
}

template <size_t COUNT, bool COMPRESS>
void serialize_contvec_flatbuf(benchmark::State& state)
{
   generatorFunc gen = generateContainerVec;
   runSerialize(state, COUNT, gen, COMPRESS);
}

template<size_t COUNT, bool COMPRESS>
void deserialize_contvec_flatbuf(benchmark::State& state)
{
   generatorFunc gen = generateContainerVec;
   readerFunc reader = [](const flatbuf_bench::Root* root) -> bool {
      long long int counter = 0;
      for (const ContainerVec* ptr: *root->cvec()) {
         benchmark::DoNotOptimize(counter += ptr->id());
      }
      return counter != 0;
   };
   runDeSerialize(state, COUNT, gen, reader, COMPRESS);
}

template <size_t COUNT, bool COMPRESS>
void serialize_contmap_flatbuf(benchmark::State& state)
{
   generatorFunc gen = generateContainerMap;
   runSerialize(state, COUNT, gen, COMPRESS);
}

template<size_t COUNT, bool COMPRESS>
void deserialize_contmap_flatbuf(benchmark::State& state)
{
   generatorFunc gen = generateContainerMap;
   readerFunc reader = [](const flatbuf_bench::Root* root) -> bool {
      long long int counter = 0;
      for (const ContainerMap* ptr: *root->cmap()) {
         benchmark::DoNotOptimize(counter += ptr->id());
      }
      return counter != 0;
   };
   runDeSerialize(state, COUNT, gen, reader, COMPRESS);
}

BENCHMARK_TEMPLATE(serialize_ints_flatbuf, 1000, small_int, false);
BENCHMARK_TEMPLATE(serialize_ints_flatbuf, 10000, small_int, false);
BENCHMARK_TEMPLATE(serialize_ints_flatbuf, 1000, small_int, true);
BENCHMARK_TEMPLATE(serialize_ints_flatbuf, 10000, small_int, true);
//BENCHMARK_TEMPLATE(serialize_ints_flatbuf, flatbuf::JSONOutputArchive, 1000);
//BENCHMARK_TEMPLATE(serialize_ints_flatbuf, flatbuf::JSONOutputArchive, 10000);

BENCHMARK_TEMPLATE(deserialize_ints_flatbuf, 1000, small_int, false);
BENCHMARK_TEMPLATE(deserialize_ints_flatbuf, 10000, small_int, false);
BENCHMARK_TEMPLATE(deserialize_ints_flatbuf, 1000, small_int, true);
BENCHMARK_TEMPLATE(deserialize_ints_flatbuf, 10000, small_int, true);
// BENCHMARK_TEMPLATE(serialize_ints_flatbuf, 1000, small_int);
// BENCHMARK_TEMPLATE(serialize_ints_flatbuf, 10000, small_int);

BENCHMARK_TEMPLATE(serialize_ints_flatbuf, 10000, integer, false);
BENCHMARK_TEMPLATE(deserialize_ints_flatbuf, 10000, integer, false);
BENCHMARK_TEMPLATE(serialize_ints_flatbuf, 10000, integer, true);
BENCHMARK_TEMPLATE(deserialize_ints_flatbuf, 10000, integer, true);

BENCHMARK_TEMPLATE(serialize_mixed_flatbuf, 1000, false);
BENCHMARK_TEMPLATE(serialize_mixed_flatbuf, 10000, false);
BENCHMARK_TEMPLATE(serialize_mixed_flatbuf, 1000, true);
BENCHMARK_TEMPLATE(serialize_mixed_flatbuf, 10000, true);
//BENCHMARK_TEMPLATE(serialize_ints_flatbuf, flatbuf::JSONOutputArchive, 1000);
//BENCHMARK_TEMPLATE(serialize_ints_flatbuf, flatbuf::JSONOutputArchive, 10000);

BENCHMARK_TEMPLATE(deserialize_mixed_flatbuf, 1000, false);
BENCHMARK_TEMPLATE(deserialize_mixed_flatbuf, 10000, false);
BENCHMARK_TEMPLATE(deserialize_mixed_flatbuf, 1000, true);
BENCHMARK_TEMPLATE(deserialize_mixed_flatbuf, 10000, true);


BENCHMARK_TEMPLATE(serialize_complex_flatbuf, 1000, false);
BENCHMARK_TEMPLATE(serialize_complex_flatbuf, 10000, false);
BENCHMARK_TEMPLATE(serialize_complex_flatbuf, 1000, true);
BENCHMARK_TEMPLATE(serialize_complex_flatbuf, 10000, true);

BENCHMARK_TEMPLATE(deserialize_complex_flatbuf, 1000, false);
BENCHMARK_TEMPLATE(deserialize_complex_flatbuf, 10000, false);
BENCHMARK_TEMPLATE(deserialize_complex_flatbuf, 1000, true);
BENCHMARK_TEMPLATE(deserialize_complex_flatbuf, 10000, true);


BENCHMARK_TEMPLATE(serialize_contvec_flatbuf, 1000, false);
BENCHMARK_TEMPLATE(serialize_contvec_flatbuf, 10000, false);
BENCHMARK_TEMPLATE(serialize_contvec_flatbuf, 1000, true);
BENCHMARK_TEMPLATE(serialize_contvec_flatbuf, 10000, true);

BENCHMARK_TEMPLATE(deserialize_contvec_flatbuf, 1000, false);
BENCHMARK_TEMPLATE(deserialize_contvec_flatbuf, 10000, false);
BENCHMARK_TEMPLATE(deserialize_contvec_flatbuf, 1000, true);
BENCHMARK_TEMPLATE(deserialize_contvec_flatbuf, 10000, true);

BENCHMARK_TEMPLATE(serialize_contmap_flatbuf, 1000, false);
BENCHMARK_TEMPLATE(serialize_contmap_flatbuf, 10000, false);
BENCHMARK_TEMPLATE(serialize_contmap_flatbuf, 1000, true);
BENCHMARK_TEMPLATE(serialize_contmap_flatbuf, 10000, true);

BENCHMARK_TEMPLATE(deserialize_contmap_flatbuf, 1000, false);
BENCHMARK_TEMPLATE(deserialize_contmap_flatbuf, 10000, false);
BENCHMARK_TEMPLATE(deserialize_contmap_flatbuf, 1000, true);
BENCHMARK_TEMPLATE(deserialize_contmap_flatbuf, 10000, true);

int main(int argc, char** argv)
{
   setup();
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}
