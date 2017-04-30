#include "benchmark/benchmark_api.h"
#include <sys/time.h>
#include <vector>
#include <chrono>
#include <cstring>

static std::vector<uint64_t> buf1;
static std::vector<uint64_t> buf2;
static constexpr int MAX_BUF_SIZE = 10*1024*1024;

static void setup() {
   buf1 = std::vector<uint64_t>(MAX_BUF_SIZE, 64);
   buf2 = std::vector<uint64_t>(MAX_BUF_SIZE, 96);
}

void TimeGTOD(benchmark::State& state) {
    struct timeval tod;
    while(state.KeepRunning()) {
        benchmark::DoNotOptimize(gettimeofday(&tod, 0));
    }
}

void TimeTime(benchmark::State& state) {
    time_t tod = 0;
    while(state.KeepRunning()) {
        benchmark::DoNotOptimize(tod = time(0));
    }
}

void TimeChrono(benchmark::State& state) {
    std::chrono::time_point<std::chrono::high_resolution_clock> tp;
    while(state.KeepRunning()) {
        benchmark::DoNotOptimize(tp = std::chrono::high_resolution_clock::now());
    }
}

void MemBench(benchmark::State& state) {
   while (state.KeepRunning()) {
      benchmark::DoNotOptimize(
         std::memcpy(buf1.data(), buf2.data(), buf1.size())
      );
   }
   state.SetBytesProcessed(state.iterations()*buf1.size()*sizeof(uint64_t));
}

void MemBenchCpp(benchmark::State& state) {
   while (state.KeepRunning()) {
      benchmark::DoNotOptimize(
         std::copy(buf1.begin(), buf1.end(), buf2.begin())
      );
   }
   state.SetBytesProcessed(state.iterations()*buf1.size()*sizeof(uint64_t));
}

BENCHMARK(TimeGTOD);
BENCHMARK(TimeTime);
BENCHMARK(TimeChrono);
BENCHMARK(MemBench);
BENCHMARK(MemBenchCpp);

int main(int argc, char** argv) {
   setup();
   ::benchmark::Initialize(&argc, argv);
   ::benchmark::RunSpecifiedBenchmarks();
}
