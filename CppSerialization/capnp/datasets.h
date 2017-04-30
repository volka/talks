#ifndef SERIALIZE_DATASET_H
#define SERIALIZE_DATASET_H

#include <map>
#include <string>
#include <vector>
#include <chrono>
#include "benchmark/benchmark_api.h"

class Timer {
public:
    using clock = std::chrono::high_resolution_clock;
    using dseconds = std::chrono::duration<double>;

    enum class State {
        INIT,
        RUNNING,
        FINISHED
    };

    Timer() : start_{clock::now()}, stop_{clock::now()}, state_{State::INIT}
    {}

    inline void start() { start_ = clock::now(); state_ = State::RUNNING; }
    inline void stop() { stop_ = clock::now(); state_ = State::FINISHED; }

    inline double getSeconds() const
    {
        using namespace std::chrono;
        switch (state_) {
        case State::INIT:
            return 0.0;
        case State::RUNNING:
            return dseconds(clock::now() - start_).count();
        case State::FINISHED:
            return dseconds(stop_ - start_).count();
        }
        return 0.0;
    }

    inline double getMicroSecs() const
    {
        using namespace std::chrono;
        switch (state_) {
        case State::INIT:
            return 0.0;
        case State::RUNNING:
            return duration_cast<microseconds>(clock::now() - start_).count();
        case State::FINISHED:
            return duration_cast<microseconds>(stop_ - start_).count();
        }
        return 0.0;
    }

    inline void clear()
    {
        state_ = State::INIT;
        start_ = clock::now();
        stop_ = clock::now();
    }

private:
    std::chrono::time_point<clock> start_;
    std::chrono::time_point<clock> stop_;
    State state_;

};

// helper for printing msg sizes in benchmark

static void printSize(benchmark::State& st, size_t bytes, int count)
{
    static char print_buf[256];
    snprintf(print_buf, sizeof(print_buf), "%lu", (bytes / st.iterations() / count));
    st.SetLabel(print_buf);
}

// constants for generators

const std::string string_short = "Fizz Buzz";
const std::string string_long = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.";
constexpr int integer = -1234567890;
constexpr int small_int = 2;
constexpr unsigned int uinteger = 1234567890;
constexpr unsigned int small_uint = 3;

enum class ENUMERATION {
    ONE,
    TWO,
    THREE,
    FOUR
};

struct ints_t {
    int64_t id;
    int64_t int1;
    int64_t int2;
    int32_t int3;
    int32_t int4;
};

struct mixed_t {
    int32_t id;
    int32_t int1;
    int64_t int2;
    uint32_t uint1;
    double float1;
    std::string text1;
    ENUMERATION enum1;
};

struct complex_t {
    int64_t id;
    std::string text1;
    std::string text2;
    std::string text3;
    int32_t int1;
    int32_t int2;
    int32_t int3;
    std::string text4;
    ENUMERATION enum1;
    double float1;
    int64_t int4;
    uint32_t uint1;
    uint64_t uint2;
    int64_t int5;
    int32_t int6;
    double float2;
    std::string text5;
};

struct container_vec_t {
    int32_t id;
    int64_t int1;
    std::string text1;
    std::vector<std::string> stringvec;
    std::vector<int64_t> intvec;
};

struct container_map_t {
    int32_t id;
    int64_t int1;
    std::string text1;
    std::map<int32_t, std::string> map1;
};

#endif
