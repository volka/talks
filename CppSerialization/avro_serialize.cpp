#include <functional>

#pragma GCC diagnostic warning "-Wtype-limits"
#include "avro/serialize.h"

#include <avro/AvroSerialize.hh>
#include <avro/Compiler.hh>
#include <avro/Decoder.hh>
#include <avro/Encoder.hh>

#include "datasets.h"

#include "benchmark/benchmark_api.h"

using generatorFunc = std::function<void(avro::EncoderPtr&, size_t)>;

constexpr static int MAX_BUF_SIZE = 16 * 1024 * 1024;
static std::vector<char> global_buffer;
static std::string schema_path;

void setupConfiguration() {
   global_buffer.resize(MAX_BUF_SIZE);
   schema_path = "../avro/serialize.avro";
}

template <int INT_VAL>
void generateInts(avro::EncoderPtr& enc, size_t count)
{
   avro_bench::root root;

   std::vector<avro_bench::ints> ints(count);
   int int_val = INT_VAL;
   int i_int = 0;
   for (size_t i = 0; i < count; ++i)
   {
      auto& current = ints[i];
      i_int = static_cast<int>(i);
      current.id = i_int;
      current.int1 = int_val++;
      current.int2 = int_val + 1;
      current.int3 = int_val + i_int;
      current.int4 = int_val + i_int + 40;
   }

   root.ints_vec.set_array(ints);
   root.mixed_vec.set_null();
   root.complex_vec.set_null();
   root.cont_vec_vec.set_null();
   root.cont_map_vec.set_null();
   avro::encode(*enc, root);
}

void generateMixed(avro::EncoderPtr& enc, size_t count)
{
   int int_val = 123456;
   std::vector<avro_bench::mixed> mixed(count);
   avro_bench::root root;

   for (size_t i = 0; i < count; ++i)
   {
      auto& current = mixed[i];
      current.id = static_cast<int>(i);
      current.int1 = int_val + 10;
      current.int2 = int_val++;
      current.uint1 = 2;
      current.float1 = 3.14;
      current.enum1 = (i % 2 == 0) ? avro_bench::ENUM::ONE : avro_bench::ENUM::TWO;
      current.text1 = string_short;

   }
   root.ints_vec.set_null();
   root.mixed_vec.set_array(mixed);
   root.complex_vec.set_null();
   root.cont_vec_vec.set_null();
   root.cont_map_vec.set_null();

   avro::encode(*enc, root);
}

void generateContainerVec(avro::EncoderPtr& enc, size_t count)
{
   std::vector<avro_bench::container_vec> contvec(count);
   avro_bench::root root;

   int int_val = 123456;
   size_t cont_count = 50;

   for (size_t i = 0; i < count; ++i)
   {
      auto& current = contvec[i];
      current.stringvec.clear();
      current.intvec.clear();

      current.id = static_cast<int>(i);
      current.int1 = int_val++;
      current.text1 = string_short;

      for (size_t j = 0; j < cont_count; ++j)
      {
         current.stringvec.push_back(string_short);
         current.intvec.push_back(int_val);
      }

   }
   root.ints_vec.set_null();
   root.mixed_vec.set_null();
   root.complex_vec.set_null();
   root.cont_vec_vec.set_array(contvec);
   root.cont_map_vec.set_null();

   avro::encode(*enc, root);
}

void generateContainerMap(avro::EncoderPtr& enc, size_t count)
{
   std::vector<avro_bench::container_map> contmap(count);
   avro_bench::root root;

   int int_val = 123456;
   size_t cont_count = 20;

   for (size_t i = 0; i < count; ++i)
   {
      auto& current = contmap[i];
      current.map1.clear();

      current.id = static_cast<int>(i);
      current.int1 = int_val++;
      current.text1 = string_short;
      for (size_t j = 0; j < cont_count; ++j)
      {
            current.map1[std::to_string(j)] = string_short;
      }
   }
   root.ints_vec.set_null();
   root.mixed_vec.set_null();
   root.complex_vec.set_null();
   root.cont_vec_vec.set_null();
   root.cont_map_vec.set_array(contmap);

   avro::encode(*enc, root);
}

void generateComplex(avro::EncoderPtr& enc, size_t count)
{
   std::vector<avro_bench::complex> complex_vec(count);
   avro_bench::root root;
   int int_val = 123456;

   for (size_t i = 0; i < count; ++i)
   {
      auto& current = complex_vec[i];
      current.id = static_cast<int>(i);
      current.int1 = int_val * int_val;
      current.int2 = int_val * 2;
      current.int3 = int_val;
      current.int4 = int_val % 100;
      current.int5 = -int_val;
      current.int6 = int_val++;
      current.enum1 = (i % 2 == 0) ? avro_bench::ENUM::ONE : avro_bench::ENUM::TWO;
      current.float1 = 3.14 * i;
      current.float2 = 3.14 * int_val;
      // AVRO does not support unsigned types ...
      current.uint1 = static_cast<int32_t>(int_val);
      current.uint2 = static_cast<int64_t>(int_val * int_val);
      current.text1 = string_short + std::to_string(i);
      current.text2 = string_short + string_short;
      current.text3 = string_long;
      current.text4 = string_short;
      current.text5 = string_long + std::to_string(int_val);

   }
   root.ints_vec.set_null();
   root.mixed_vec.set_null();
   root.complex_vec.set_array(complex_vec);
   root.cont_vec_vec.set_null();
   root.cont_map_vec.set_null();

   avro::encode(*enc, root);
}

// generic serialize function
void runSerialize(benchmark::State& state, size_t count, generatorFunc& generator, avro::EncoderPtr& enc)
{
   size_t generated = 0;

   // the memoryOutputStream would normally use several chunks, but this way we force it to use
   // one big buffer, which has the additional advantage that we can backup() to the beginning for
   // each iteration
   std::unique_ptr<avro::OutputStream> out = avro::memoryOutputStream(MAX_BUF_SIZE);

   while (state.KeepRunning())
   {
      out->backup(out->byteCount());
      enc->init(*out);
      generator(enc, count);
      enc->flush();
      generated += out->byteCount();
   }
   state.SetBytesProcessed(generated);
   state.SetItemsProcessed(state.iterations() * count);

   printSize(state, generated, static_cast<int>(count));
}

// generic deserialize test function, relatively huge overhead through generation may skew results
template <class TYPE>
void runDeSerialize(benchmark::State& state, size_t count, generatorFunc& generator,
                    avro::EncoderPtr& enc, avro::DecoderPtr& dec)
{
   size_t bytes = 0;
   //boost::iostreams::stream_buffer<BidiExtBufferDevice> buffer(
   //    const_cast<char*>(global_buffer.data()), global_buffer.size());

   std::unique_ptr<avro::OutputStream> out = avro::memoryOutputStream(MAX_BUF_SIZE);
   { // the archiver writes the closing marker in it's destructor. this is essential for JSON
      enc->init(*out);
      generator(enc, count);
      enc->flush();
   }

   std::unique_ptr<avro::InputStream> in = avro::memoryInputStream(*out);

   avro_bench::root root;

   while (state.KeepRunning())
   {
      dec->init(*in);
      avro::decode(*dec, root);
//      std::cout << "decoding ... current bytes: " << bytes  << " bc: " << in->byteCount() << std::endl;
      bytes += in->byteCount();
      in->backup(in->byteCount());
   }
   state.SetBytesProcessed(bytes);
   state.SetItemsProcessed(state.iterations() * count);
   printSize(state, bytes, static_cast<int>(count));
}

// type specific benchmark entry points
template <int COUNT>
void serialize_ints_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateInts<small_int>;
   auto encoder = avro::binaryEncoder();
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_ints_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateInts<small_int>;
   auto encoder = avro::binaryEncoder();
   auto decoder = avro::binaryDecoder();
   runDeSerialize<avro_bench::ints>(state, COUNT, gen, encoder, decoder);
}

template <int COUNT>
void serialize_ints_avro_json(benchmark::State& state)
{
   generatorFunc gen = generateInts<small_int>;
   avro::ValidSchema schema = avro::compileJsonSchemaFromFile(schema_path.c_str());
   auto encoder = avro::jsonEncoder(schema);
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_ints_avro_json(benchmark::State& state)
{
   generatorFunc gen = generateInts<small_int>;
   avro::ValidSchema schema = avro::compileJsonSchemaFromFile(schema_path.c_str());
   auto encoder = avro::jsonEncoder(schema);
   auto decoder = avro::jsonDecoder(schema);
   runDeSerialize<avro_bench::ints>(state, COUNT, gen, encoder, decoder);
}
template <int COUNT>

void serialize_large_ints_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateInts<integer>;
   auto encoder = avro::binaryEncoder();
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_large_ints_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateInts<integer>;
   auto encoder = avro::binaryEncoder();
   auto decoder = avro::binaryDecoder();
   runDeSerialize<avro_bench::ints>(state, COUNT, gen, encoder, decoder);
}

template <int COUNT>
void serialize_mixed_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateMixed;
   auto encoder = avro::binaryEncoder();
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_mixed_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateMixed;
   auto encoder = avro::binaryEncoder();
   auto decoder = avro::binaryDecoder();
   runDeSerialize<avro_bench::mixed>(state, COUNT, gen, encoder, decoder);
}

template <int COUNT>
void serialize_mixed_avro_json(benchmark::State& state)
{
   generatorFunc gen = generateMixed;
   avro::ValidSchema schema = avro::compileJsonSchemaFromFile(schema_path.c_str());
   auto encoder = avro::jsonEncoder(schema);
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_mixed_avro_json(benchmark::State& state)
{
   generatorFunc gen = generateMixed;
   avro::ValidSchema schema = avro::compileJsonSchemaFromFile(schema_path.c_str());
   auto encoder = avro::jsonEncoder(schema);
   auto decoder = avro::jsonDecoder(schema);
   runDeSerialize<avro_bench::mixed>(state, COUNT, gen, encoder, decoder);
}

template <int COUNT>
void serialize_complex_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateComplex;
   auto encoder = avro::binaryEncoder();
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_complex_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateComplex;
   auto encoder = avro::binaryEncoder();
   auto decoder = avro::binaryDecoder();
   runDeSerialize<avro_bench::complex>(state, COUNT, gen, encoder, decoder);
}

template <int COUNT>
void serialize_complex_avro_json(benchmark::State& state)
{
   generatorFunc gen = generateComplex;
   avro::ValidSchema schema = avro::compileJsonSchemaFromFile(schema_path.c_str());
   auto encoder = avro::jsonEncoder(schema);
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_complex_avro_json(benchmark::State& state)
{
   generatorFunc gen = generateComplex;
   avro::ValidSchema schema = avro::compileJsonSchemaFromFile(schema_path.c_str());
   auto encoder = avro::jsonEncoder(schema);
   auto decoder = avro::jsonDecoder(schema);
   runDeSerialize<avro_bench::complex>(state, COUNT, gen, encoder, decoder);
}

template <int COUNT>
void serialize_contvec_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateContainerVec;
   auto encoder = avro::binaryEncoder();
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_contvec_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateContainerVec;
   auto encoder = avro::binaryEncoder();
   auto decoder = avro::binaryDecoder();
   runDeSerialize<avro_bench::container_vec>(state, COUNT, gen, encoder, decoder);
}

template <int COUNT>
void serialize_contvec_avro_json(benchmark::State& state)
{
   generatorFunc gen = generateContainerVec;
   avro::ValidSchema schema = avro::compileJsonSchemaFromFile(schema_path.c_str());
   auto encoder = avro::jsonEncoder(schema);
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_contvec_avro_json(benchmark::State& state)
{
   generatorFunc gen = generateContainerVec;
   avro::ValidSchema schema = avro::compileJsonSchemaFromFile(schema_path.c_str());
   auto encoder = avro::jsonEncoder(schema);
   auto decoder = avro::jsonDecoder(schema);
   runDeSerialize<avro_bench::container_vec>(state, COUNT, gen, encoder, decoder);
}

template <int COUNT>
void serialize_contmap_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateContainerMap;
   auto encoder = avro::binaryEncoder();
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_contmap_avro_bin(benchmark::State& state)
{
   generatorFunc gen = generateContainerMap;
   auto encoder = avro::binaryEncoder();
   auto decoder = avro::binaryDecoder();
   runDeSerialize<avro_bench::container_map>(state, COUNT, gen, encoder, decoder);
}

template <int COUNT>
void serialize_contmap_avro_json(benchmark::State& state)
{
   generatorFunc gen = generateContainerMap;
   avro::ValidSchema schema = avro::compileJsonSchemaFromFile(schema_path.c_str());
   auto encoder = avro::jsonEncoder(schema);
   runSerialize(state, COUNT, gen, encoder);
}

template <int COUNT>
void deserialize_contmap_avro_json(benchmark::State& state)
{
   generatorFunc gen = generateContainerMap;
   avro::ValidSchema schema = avro::compileJsonSchemaFromFile(schema_path.c_str());
   auto encoder = avro::jsonEncoder(schema);
   auto decoder = avro::jsonDecoder(schema);
   runDeSerialize<avro_bench::container_map>(state, COUNT, gen, encoder, decoder);
}

BENCHMARK_TEMPLATE(serialize_ints_avro_bin, 1000);
BENCHMARK_TEMPLATE(serialize_ints_avro_bin, 10000);
BENCHMARK_TEMPLATE(serialize_ints_avro_json, 1000);
BENCHMARK_TEMPLATE(serialize_ints_avro_json, 10000);

BENCHMARK_TEMPLATE(deserialize_ints_avro_bin, 1000);
BENCHMARK_TEMPLATE(deserialize_ints_avro_bin, 10000);
BENCHMARK_TEMPLATE(deserialize_ints_avro_json, 1000);
BENCHMARK_TEMPLATE(deserialize_ints_avro_json, 10000);

BENCHMARK_TEMPLATE(serialize_mixed_avro_bin, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_avro_bin, 10000);
BENCHMARK_TEMPLATE(serialize_mixed_avro_json, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_avro_json, 10000);

BENCHMARK_TEMPLATE(deserialize_mixed_avro_bin, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_avro_bin, 10000);
BENCHMARK_TEMPLATE(deserialize_mixed_avro_json, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_avro_json, 10000);

BENCHMARK_TEMPLATE(serialize_complex_avro_bin, 1000);
BENCHMARK_TEMPLATE(serialize_complex_avro_bin, 10000);
BENCHMARK_TEMPLATE(serialize_complex_avro_json, 1000);
BENCHMARK_TEMPLATE(serialize_complex_avro_json, 10000);

BENCHMARK_TEMPLATE(deserialize_complex_avro_bin, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_avro_bin, 10000);
BENCHMARK_TEMPLATE(deserialize_complex_avro_json, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_avro_json, 10000);

BENCHMARK_TEMPLATE(serialize_contvec_avro_bin, 1000);
BENCHMARK_TEMPLATE(serialize_contvec_avro_bin, 10000);
BENCHMARK_TEMPLATE(serialize_contvec_avro_json, 1000);
BENCHMARK_TEMPLATE(serialize_contvec_avro_json, 10000);

BENCHMARK_TEMPLATE(deserialize_contvec_avro_bin, 1000);
BENCHMARK_TEMPLATE(deserialize_contvec_avro_bin, 10000);
BENCHMARK_TEMPLATE(deserialize_contvec_avro_json, 1000);
BENCHMARK_TEMPLATE(deserialize_contvec_avro_json, 10000);

BENCHMARK_TEMPLATE(serialize_contmap_avro_bin, 1000);
BENCHMARK_TEMPLATE(serialize_contmap_avro_bin, 10000);
BENCHMARK_TEMPLATE(serialize_contmap_avro_json, 1000);
BENCHMARK_TEMPLATE(serialize_contmap_avro_json, 10000);

BENCHMARK_TEMPLATE(deserialize_contmap_avro_bin, 1000);
BENCHMARK_TEMPLATE(deserialize_contmap_avro_bin, 10000);
BENCHMARK_TEMPLATE(deserialize_contmap_avro_json, 1000);
BENCHMARK_TEMPLATE(deserialize_contmap_avro_json, 10000);

int main(int argc, char** argv)
{
   setupConfiguration();
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}
