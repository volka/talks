#include <vector>
#include <iostream>
#include <functional>
#include "benchmark/benchmark_api.h"

#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <capnp/serialize-async.h>

#include "datasets.h"
#include "capnp/serialize.capnp.h"

#define COUNT_MSGSIZE

using writerFunc = std::function<void(kj::OutputStream&, capnp::MessageBuilder&)>;
using generatorFunc = std::function<size_t(kj::OutputStream&, writerFunc, size_t)>;

constexpr static int MAX_BUF_SIZE = 40*1024*1024;
static std::vector<kj::byte> global_buffer;

static void setup()
{
   global_buffer.resize(MAX_BUF_SIZE);
}

size_t generateInts(int int_val, kj::OutputStream& out, writerFunc writer, size_t count)
{
#ifdef COUNT_MSGSIZE
   size_t generated = 0;
#endif
   capnp::MallocMessageBuilder builder;

   auto msg = builder.initRoot<capnp_bench::IntList>();
   auto int_builder = msg.initInts(static_cast<unsigned int>(count));
   for (int i = 0; i < static_cast<int>(count); ++i)
   {
      auto current = int_builder[static_cast<unsigned int>(i)];
      current.setId(i);
      current.setInt1(int_val++);
      current.setInt2(int_val+1);
      current.setInt3(int_val+i);
      current.setInt4(int_val+i+40);
   }
   writer(out, builder);
#ifdef COUNT_MSGSIZE
   generated += capnp::computeSerializedSizeInWords(builder);
   return generated;
#else
   return 0;
#endif
}

void runSerialize(benchmark::State& st, size_t count, generatorFunc gen, writerFunc writer)
{
   size_t generated = 0;
   while (st.KeepRunning()) {
      kj::ArrayPtr<kj::byte> arr(global_buffer.data(), global_buffer.size());
      kj::ArrayOutputStream out(arr);
      generated += gen(out, writer, count);
   }
   st.SetBytesProcessed(generated);
   st.SetItemsProcessed(st.iterations()*count);
   printSize(st, generated, static_cast<int>(count));
}

/*
template<class OutType, class InType>
void runDeSerialize(benchmark::State& st, size_t count, generatorFunc gen)
{
}
*/

template<class Writer, size_t COUNT, int INT_VAL>
void serialize_ints_capnp(benchmark::State& st)
{
   Writer w;
   runSerialize(st, COUNT,
                std::bind(generateInts, INT_VAL,
                          std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
                w);
}

BENCHMARK_TEMPLATE(serialize_ints_capnp, capnp::writeMessage, 1000, small_int);

int main(int argc, char** argv) {
   setup();
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}
