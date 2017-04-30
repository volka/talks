
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

#include <functional>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>

#ifdef BOOST_SERIALIZATION_NVP
#undef BOOST_SERIALIZATION_NVP
#endif
#define BOOST_SERIALIZATION_NVP(name) name

#include "cereal/serialize.h"

#include "benchmark/benchmark_api.h"

template <class ARCHIVE>
using generatorFunc = std::function<void(ARCHIVE&, int)>;

constexpr static int MAX_BUF_SIZE = 8 * 1024 * 1024;
static std::vector<char> global_buffer;

// helper buffer class to work with cereal
// the usual buffers do not provide easy access to # of bytes written
struct BidiExtBufferDevice
{
   using char_type = char;
   using category = boost::iostreams::bidirectional_device_tag;

   char_type* ext_buffer;
   std::streamsize ext_buffer_size;
   char_type* read_ptr;
   char_type* write_ptr;

   inline std::streamsize readBytesLeft() const { return std::abs(write_ptr - read_ptr); }

   inline std::streamsize writeBytesLeft() const
   {
      return ext_buffer_size - std::abs(write_ptr - ext_buffer);
   }

   inline std::streamsize bytesWritten() const { return std::abs(write_ptr - ext_buffer); }

   inline std::streamsize bytesRead() const { return std::abs(read_ptr - ext_buffer); }

   // reset the read pointer
   inline void reOpen() { read_ptr = ext_buffer; }
   inline void reset()
   {
      // bzero(ext_buffer, ext_buffer_size);
      read_ptr = ext_buffer;
      write_ptr = ext_buffer;
   }

   BidiExtBufferDevice(char_type* buf, size_t size)
       : ext_buffer(buf), ext_buffer_size(size), read_ptr(buf), write_ptr(buf)
   {
   }

   std::streamsize read(char_type* s, std::streamsize n)
   {
      auto available = readBytesLeft();
      std::streamsize to_read = std::min(n, available);
      if (available <= 0)
         return -1;
      memcpy(s, read_ptr, to_read);
      read_ptr += to_read;
      return to_read;
   }

   std::streamsize write(const char* s, std::streamsize n)
   {
      auto available = writeBytesLeft();
      if (available <= n)
         throw std::runtime_error("Insufficient space left to write to ExtBuffer");
      memcpy(write_ptr, s, n);
      write_ptr += n;
      return n;
   }
};

void setupConfiguration() { global_buffer.resize(MAX_BUF_SIZE); }

template <class ARCHIVE, int INT_VAL>
void generateInts(ARCHIVE& archive, int count)
{
   cereal_bench::Ints current;
   int int_val = INT_VAL;

   for (int i = 0; i < count; ++i)
   {
      current.id = i;
      current.int1 = int_val++;
      current.int2 = int_val + 1;
      current.int3 = int_val + i;
      current.int4 = int_val + i + 40;

      archive << current;
   }
}

template <class ARCHIVE>
void generateMixed(ARCHIVE& archive, int count)
{
   int int_val = 123456;
   cereal_bench::Mixed current;
   for (int i = 0; i < count; ++i)
   {
      current.id = i;
      current.int1 = int_val + 10;
      current.int2 = int_val++;
      current.uint1 = 2;
      current.float1 = 3.14;
      current.enum1 = (i % 2 == 0) ? ::ENUMERATION::ONE : ::ENUMERATION::TWO;
      current.text1 = string_short;
      archive << current;
   }
}

template <class ARCHIVE>
void generateContainerVec(ARCHIVE& archive, int count)
{
   cereal_bench::Container_Vec current;

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

      archive << current;
   }
}

template <class ARCHIVE>
void generateContainerMap(ARCHIVE& archive, int count)
{
   cereal_bench::Container_Map current;
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
      archive << current;
   }
}

template <class ARCHIVE>
void generateComplex(ARCHIVE& archive, int count)
{
   cereal_bench::Complex current;
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
      current.enum1 = (i % 2 == 0) ? ::ENUMERATION::ONE : ::ENUMERATION::TWO;
      current.float1 = 3.14 * i;
      current.float2 = 3.14 * int_val;
      current.uint1 = static_cast<uint32_t>(int_val);
      current.uint2 = static_cast<uint64_t>(int_val * int_val);
      current.text1 = string_short + std::to_string(i);
      current.text2 = string_short + string_short;
      current.text3 = string_long;
      current.text4 = string_short;
      current.text5 = string_long + std::to_string(int_val);

      archive << current;
   }
}

// generic serialize function
template <class ARCHIVE>
void runSerialize(benchmark::State& state, int count, generatorFunc<ARCHIVE>& generator)
{
   int generated = 0;
   Timer t;
   boost::iostreams::stream_buffer<BidiExtBufferDevice> buffer(
       const_cast<char*>(global_buffer.data()), global_buffer.size());
   std::ostream out(&buffer);

   while (state.KeepRunning())
   {
      { // the archiver writes the closing marker in it's destructor. this is essential for JSON
         ARCHIVE archiver(out);
         generator(archiver, count);
      }
      // ostreams don't always flush ...
      out.flush();
      generated += buffer->bytesWritten();
      buffer->reset();
   }
   state.SetBytesProcessed(generated);
   state.SetItemsProcessed(state.iterations() * count);

   printSize(state, generated, count);
}

// generic deserialize test function, relatively huge overhead through generation may skew results
template <class INARCHIVE, class OUTARCHIVE, class TYPE>
void runDeSerialize(benchmark::State& state, int count, generatorFunc<OUTARCHIVE>& generator)
{
   int bytes = 0;

   std::vector<TYPE> data(count);
   boost::iostreams::stream_buffer<BidiExtBufferDevice> buffer(
       const_cast<char*>(global_buffer.data()), global_buffer.size());
   std::ostream out(&buffer);
   { // the archiver writes the closing marker in it's destructor. this is essential for JSON
      OUTARCHIVE oarchiver(out);
      generator(oarchiver, count);
   }
   // ostreams don't always flush ...
   out.flush();

   while (state.KeepRunning())
   {
      buffer->reOpen();
      std::istream in(&buffer);

      INARCHIVE iarchiver(in);
      for (auto& current : data)
      {
         iarchiver >> current;
      }
      bytes += buffer->bytesRead();
   }
   state.SetBytesProcessed(bytes);
   state.SetItemsProcessed(state.iterations() * count);
   printSize(state, bytes, count);
}

// type specific benchmark entry points
template <class ARCHIVE, int COUNT>
void serialize_ints_cereal(benchmark::State& state)
{
   generatorFunc<ARCHIVE> gen = generateInts<ARCHIVE, small_int>;
   runSerialize(state, COUNT, gen);
}

template <class INARCHIVE, class OUTARCHIVE, int COUNT>
void deserialize_ints_cereal(benchmark::State& state)
{
   generatorFunc<OUTARCHIVE> gen = generateInts<OUTARCHIVE, small_int>;
   runDeSerialize<INARCHIVE, OUTARCHIVE, cereal_bench::Ints>(state, COUNT, gen);
}

template <class ARCHIVE, int COUNT>
void serialize_large_ints_cereal(benchmark::State& state)
{
   generatorFunc<ARCHIVE> gen = generateInts<ARCHIVE, integer>;
   runSerialize(state, COUNT, gen);
}

template <class INARCHIVE, class OUTARCHIVE, int COUNT>
void deserialize_large_ints_cereal(benchmark::State& state)
{
   generatorFunc<OUTARCHIVE> gen = generateInts<OUTARCHIVE, integer>;
   runDeSerialize<INARCHIVE, OUTARCHIVE, cereal_bench::Ints>(state, COUNT, gen);
}

template <class ARCHIVE, int COUNT>
void serialize_mixed_cereal(benchmark::State& state)
{
   generatorFunc<ARCHIVE> gen = generateMixed<ARCHIVE>;
   runSerialize(state, COUNT, gen);
}

template <class INARCHIVE, class OUTARCHIVE, int COUNT>
void deserialize_mixed_cereal(benchmark::State& state)
{
   generatorFunc<OUTARCHIVE> gen = generateMixed<OUTARCHIVE>;
   runDeSerialize<INARCHIVE, OUTARCHIVE, cereal_bench::Mixed>(state, COUNT, gen);
}

template <class ARCHIVE, int COUNT>
void serialize_complex_cereal(benchmark::State& state)
{
   generatorFunc<ARCHIVE> gen = generateComplex<ARCHIVE>;
   runSerialize(state, COUNT, gen);
}

template <class INARCHIVE, class OUTARCHIVE, int COUNT>
void deserialize_complex_cereal(benchmark::State& state)
{
   generatorFunc<OUTARCHIVE> gen = generateComplex<OUTARCHIVE>;
   runDeSerialize<INARCHIVE, OUTARCHIVE, cereal_bench::Complex>(state, COUNT, gen);
}

template <class ARCHIVE, int COUNT>
void serialize_contvec_cereal(benchmark::State& state)
{
   generatorFunc<ARCHIVE> gen = generateContainerVec<ARCHIVE>;
   runSerialize(state, COUNT, gen);
}

template <class INARCHIVE, class OUTARCHIVE, int COUNT>
void deserialize_contvec_cereal(benchmark::State& state)
{
   generatorFunc<OUTARCHIVE> gen = generateContainerVec<OUTARCHIVE>;
   runDeSerialize<INARCHIVE, OUTARCHIVE, cereal_bench::Container_Vec>(state, COUNT, gen);
}

template <class ARCHIVE, int COUNT>
void serialize_contmap_cereal(benchmark::State& state)
{
   generatorFunc<ARCHIVE> gen = generateContainerMap<ARCHIVE>;
   runSerialize(state, COUNT, gen);
}

template <class INARCHIVE, class OUTARCHIVE, int COUNT>
void deserialize_contmap_cereal(benchmark::State& state)
{
   generatorFunc<OUTARCHIVE> gen = generateContainerMap<OUTARCHIVE>;
   runDeSerialize<INARCHIVE, OUTARCHIVE, cereal_bench::Container_Map>(state, COUNT, gen);
}

BENCHMARK_TEMPLATE(serialize_ints_cereal, cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_ints_cereal, cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_ints_cereal, cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_ints_cereal, cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_ints_cereal, cereal::JSONOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_ints_cereal, cereal::JSONOutputArchive, 10000);

BENCHMARK_TEMPLATE(deserialize_ints_cereal, cereal::BinaryInputArchive, cereal::BinaryOutputArchive,
                   1000);
BENCHMARK_TEMPLATE(deserialize_ints_cereal, cereal::BinaryInputArchive, cereal::BinaryOutputArchive,
                   10000);
BENCHMARK_TEMPLATE(deserialize_ints_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_ints_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_ints_cereal, cereal::JSONInputArchive, cereal::JSONOutputArchive,
                   1000);
BENCHMARK_TEMPLATE(deserialize_ints_cereal, cereal::JSONInputArchive, cereal::JSONOutputArchive,
                   10000);

BENCHMARK_TEMPLATE(serialize_large_ints_cereal, cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_large_ints_cereal, cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_large_ints_cereal, cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_large_ints_cereal, cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_large_ints_cereal, cereal::JSONOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_large_ints_cereal, cereal::JSONOutputArchive, 10000);

BENCHMARK_TEMPLATE(deserialize_large_ints_cereal, cereal::BinaryInputArchive,
                   cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_large_ints_cereal, cereal::BinaryInputArchive,
                   cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_large_ints_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_large_ints_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_large_ints_cereal, cereal::JSONInputArchive,
                   cereal::JSONOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_large_ints_cereal, cereal::JSONInputArchive,
                   cereal::JSONOutputArchive, 10000);

BENCHMARK_TEMPLATE(serialize_mixed_cereal, cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_cereal, cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_mixed_cereal, cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_cereal, cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_mixed_cereal, cereal::JSONOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_mixed_cereal, cereal::JSONOutputArchive, 10000);

BENCHMARK_TEMPLATE(deserialize_mixed_cereal, cereal::BinaryInputArchive,
                   cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_cereal, cereal::BinaryInputArchive,
                   cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_mixed_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_mixed_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_mixed_cereal, cereal::JSONInputArchive, cereal::JSONOutputArchive,
                   1000);
BENCHMARK_TEMPLATE(deserialize_mixed_cereal, cereal::JSONInputArchive, cereal::JSONOutputArchive,
                   10000);

BENCHMARK_TEMPLATE(serialize_complex_cereal, cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_complex_cereal, cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_complex_cereal, cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_complex_cereal, cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_complex_cereal, cereal::JSONOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_complex_cereal, cereal::JSONOutputArchive, 10000);

BENCHMARK_TEMPLATE(deserialize_complex_cereal, cereal::BinaryInputArchive,
                   cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_cereal, cereal::BinaryInputArchive,
                   cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_complex_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_complex_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_complex_cereal, cereal::JSONInputArchive, cereal::JSONOutputArchive,
                   1000);
BENCHMARK_TEMPLATE(deserialize_complex_cereal, cereal::JSONInputArchive, cereal::JSONOutputArchive,
                   10000);

BENCHMARK_TEMPLATE(serialize_contvec_cereal, cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_contvec_cereal, cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_contvec_cereal, cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_contvec_cereal, cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_contvec_cereal, cereal::JSONOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_contvec_cereal, cereal::JSONOutputArchive, 10000);

BENCHMARK_TEMPLATE(deserialize_contvec_cereal, cereal::BinaryInputArchive,
                   cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_contvec_cereal, cereal::BinaryInputArchive,
                   cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_contvec_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_contvec_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_contvec_cereal, cereal::JSONInputArchive, cereal::JSONOutputArchive,
                   1000);
BENCHMARK_TEMPLATE(deserialize_contvec_cereal, cereal::JSONInputArchive, cereal::JSONOutputArchive,
                   10000);

BENCHMARK_TEMPLATE(serialize_contmap_cereal, cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_contmap_cereal, cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_contmap_cereal, cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_contmap_cereal, cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(serialize_contmap_cereal, cereal::JSONOutputArchive, 1000);
BENCHMARK_TEMPLATE(serialize_contmap_cereal, cereal::JSONOutputArchive, 10000);

BENCHMARK_TEMPLATE(deserialize_contmap_cereal, cereal::BinaryInputArchive,
                   cereal::BinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_contmap_cereal, cereal::BinaryInputArchive,
                   cereal::BinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_contmap_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 1000);
BENCHMARK_TEMPLATE(deserialize_contmap_cereal, cereal::PortableBinaryInputArchive,
                   cereal::PortableBinaryOutputArchive, 10000);
BENCHMARK_TEMPLATE(deserialize_contmap_cereal, cereal::JSONInputArchive, cereal::JSONOutputArchive,
                   1000);
BENCHMARK_TEMPLATE(deserialize_contmap_cereal, cereal::JSONInputArchive, cereal::JSONOutputArchive,
                   10000);

int main(int argc, char** argv)
{
   setupConfiguration();
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}
