#include "datasets.h"
#include "serialize_reflection.h"

#include <iostream>
#include <chrono>

#include <bond/core/bond.h>
#include <bond/stream/output_buffer.h>
#include <bond/protocol/simple_json_reader.h>
#include <bond/protocol/simple_json_reader_impl.h>
#include <bond/protocol/simple_json_writer.h>

using namespace uti::serialize;

template<class T>
using generatorFunc = std::vector<T>(*)(size_t);

std::vector<ints> generate_ints(size_t count, int int_val)
{
    std::vector<ints> result(count);
    size_t id = 0;
    while (id < count) {
        auto& current = result[id];
        current.id = id++;
        current.int1 = int_val++;
        current.int2 = int_val+1;
        current.int3 = int_val+id;
        current.int4 = int_val+id+40;
    }
    return result;
}

std::vector<mixed> generate_mixed(size_t count)
{
    std::vector<mixed> result(count);
    size_t id = 0;
    int int_val = 123456789;
    while (id < count) {
        auto& m = result[id];
        m.id = id++;
        m.int1 = int_val+10;
        m.int2 = int_val++;
        m.float1 = 3.14;
        m.enum1 = (id % 2 == 0) ? Enum1::ONE : Enum1::TWO;
        m.uint1 = 2;
        m.text1 = string_short;
    }
    return result;
}

std::vector<container_vec> generate_contvec(size_t count)
{
    std::vector<container_vec> result(count);
    size_t id = 0;
    int int_val = 123456;
    int cont_count = 50;
    while(id < count) {
        auto& current = result[id];
        current.id = id++;
        current.int1 = int_val++;
        current.text1 = string_short;
        for (int i = 0; i < cont_count; ++i) {
            current.stringvec.push_back(string_short);
            current.intvec.push_back(int_val);
        }
    }
    return result;
}

std::vector<container_map> generate_contmap(size_t count)
{
    std::vector<container_map> result(count);
    size_t id = 0;
    int int_val = 123456;
    int cont_count = 20;
    while(id < count) {
        auto& current = result[id];
        current.id = id++;
        current.int1 = int_val++;
        current.text1 = string_short;
        for (int i = 0; i < cont_count; ++i) {
            current.map1.insert(std::make_pair(i, string_short));

        }
    }
    return result;
}

std::vector<complex> generate_complex(size_t count)
{
    std::vector<complex> result(count);
    size_t id = 0;
    int int_val = 123456;
    while(id < count) {
        complex& current = result[id];
        current.id = id++;
        current.int1 = int_val*int_val;
        current.int2 = int_val*2;
        current.int3 = int_val;
        current.int4 = int_val%100;
        current.int5 = -int_val;
        current.int6 = int_val++;
        current.enum1 = (id % 2 == 0) ? Enum1::ONE : Enum1::TWO;
        current.float1 = 3.14*id;
        current.float2 = 3.14*int_val;
        current.uint1 = static_cast<uint32_t>(int_val);
        current.uint2 = static_cast<uint64_t>(int_val*int_val);
        current.text1 = string_short + std::to_string(id);
        current.text2 = string_short + string_short;
        current.text3 = string_long;
        current.text4 = string_short;
        current.text5 = string_long + std::to_string(int_val);
    }
    return result;
}

void printHeader()
{
    std::cout << "testcase, packet_count, serialize_time_microsec, deserialize_time_microsec, whole_buffer_size, serialize_mbps, deserialize_mbps, per_packet_size" << std::endl;
}

// timings in microseconds, size in bytes
void printStats(const std::string& title, int serialize_ms, int deserialize_ms, int count, int whole_buffer_size)
{
    std::cout << title << ", " << count << ", " << serialize_ms << ", " << deserialize_ms << ", "
              << whole_buffer_size << ", "
              << 1.0/serialize_ms*whole_buffer_size << ", " << 1.0/deserialize_ms*whole_buffer_size << ", "
              << whole_buffer_size / count << std::endl;
}

template <class T>
int test_ints_serialize(T& writer, int count, int initial = small_int)
{
    auto objs = generate_ints(count, initial);

    Timer t;
    t.start();
    for (auto& obj: objs) {
        Serialize(obj, writer);
    }
    t.stop();
    return t.getMicroSecs();
}

template<class T>
int test_ints_deserialize(T& reader, int count)
{
    std::vector<ints> result(count);

    Timer t;
    t.start();
    for (auto& res: result) {
        Deserialize(reader, res);
    }
    t.stop();
    return t.getMicroSecs();
}

template<class T>
int test_mixed_serialize(T& writer, int count)
{
    auto objs = generate_mixed(count);

    Timer t;
    t.start();
    for (auto& obj: objs) {
        Serialize(obj, writer);
    }
    t.stop();
    return t.getMicroSecs();
}

template<class T>
int test_mixed_deserialize(T& reader, int count)
{
    std::vector<mixed> result(count);

    Timer t;
    t.start();
    for (auto& res: result) {
        Deserialize(reader, res);
    }
    t.stop();
    return t.getMicroSecs();
}

template<class T>
int test_contvec_serialize(T& writer, int count)
{
    auto objs = generate_contvec(count);

    Timer t;
    t.start();
    for (auto& obj: objs) {
        Serialize(obj, writer);
    }
    t.stop();
    return t.getMicroSecs();
}

template<class T>
int test_contvec_deserialize(T& reader, int count)
{
    std::vector<container_vec> result(count);

    Timer t;
    t.start();
    for (auto& res: result) {
        Deserialize(reader, res);
    }
    t.stop();
    return t.getMicroSecs();
}

template<class T>
int test_contmap_serialize(T& writer, int count)
{
    auto objs = generate_contmap(count);

    Timer t;
    t.start();
    for (auto& obj: objs) {
        Serialize(obj, writer);
    }
    t.stop();
    return t.getMicroSecs();
}

template<class T>
int test_contmap_deserialize(T& reader, int count)
{
    std::vector<container_map> result(count);

    Timer t;
    t.start();
    for (auto& res: result) {
        Deserialize(reader, res);
    }
    t.stop();
    return t.getMicroSecs();
}

template<class MsgType, class T>
int test_serialize(T& writer, int count, generatorFunc<MsgType> generator)
{
    auto objs = generator(count);

    Timer t;
    t.start();
    for (auto& obj: objs) {
        Serialize(obj, writer);
    }
    t.stop();
    return t.getMicroSecs();
}

template<class MsgType, class T>
int test_deserialize(T& reader, int count)
{
    std::vector<MsgType> result(count);

    Timer t;
    t.start();
    for (auto& res: result) {
        Deserialize(reader, res);
    }
    t.stop();
    return t.getMicroSecs();
}

int main(int argc, char** argv)
{
    int msg_count = 10000;
    int testcase = 0;
    if (argc >= 3) {
        msg_count = atoi(argv[1]);
        testcase = atoi(argv[2]);
    } else {
        std::cout << "Usage: " << argv[0] << " <Message Count> <Testcase No.>" << std::endl << std::endl;
        std::cout << "Testcases:" << std::endl;
        std::cout << "\t1: ints with small values, CompactBinary" << std::endl;
        std::cout << "\t2: ints with large values, CompactBinary" << std::endl;
        std::cout << "\t3: mixed, CompactBinary" << std::endl;
        std::cout << "\t4: vector container, CompactBinary" << std::endl;
        std::cout << "\t5: map container, CompactBinary" << std::endl;
        std::cout << "\t6: complex data, CompactBinary" << std::endl;
        std::cout << "\t11: ints with small values, FastBinary" << std::endl;
        std::cout << "\t12: ints with large values, FastBinary" << std::endl;
        std::cout << "\t13: mixed, FastBinary" << std::endl;
        std::cout << "\t14: vector container, FastBinary" << std::endl;
        std::cout << "\t15: map container, FastBinary" << std::endl;
        std::cout << "\t16: complex data, FastBinary" << std::endl;
        std::cout << "\t21: ints with small values, Json" << std::endl;
        std::cout << "\t22: ints with large values, Json" << std::endl;
        std::cout << "\t23: mixed, Json" << std::endl;
        std::cout << "\t24: vector container, Json" << std::endl;
        std::cout << "\t25: map container, Json" << std::endl;
        std::cout << "\t26: complex data, Json" << std::endl;
        std::cout << "\t99: RUN ALL TESTS" << std::endl;
        return -1;
    }

    int serialize_time = 0;
    int deserialize_time = 0;
    int whole_buffer_size = 0;
    std::string testcase_name;

    std::vector<int> testcases;
    if (testcase == 99) {
        testcases = {1,2,3,4,5,6,11,12,13,14,15,16,21,22,23,24,25,26};
    } else {
        testcases.push_back(testcase);
        if (argc > 3) {
            for (int i = 3; i < argc; ++i) {
                testcases.push_back(atoi(argv[i]));
            }
        }
    }

    printHeader();
    for (int t: testcases)
    {
        bond::OutputBuffer output;
        bond::OutputBuffer out_json;
        bond::OutputBuffer out_fastbin;
        bond::CompactBinaryWriter<bond::OutputBuffer> writer(output);
        bond::SimpleJsonWriter<bond::OutputBuffer> json_writer(out_json);
        bond::FastBinaryWriter<bond::OutputBuffer> fast_writer(out_fastbin);

        switch (t) {
            case 1:
            {
                testcase_name = "ints-small CompactBinary";
                serialize_time = test_ints_serialize(writer, msg_count, small_int);
                bond::blob data = output.GetBuffer();
                whole_buffer_size = data.size();
                bond::CompactBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_ints_deserialize(reader, msg_count);
                break;
            }
            case 2:
            {
                testcase_name = "ints-large CompactBinary";
                serialize_time = test_ints_serialize(writer, msg_count, integer);
                bond::blob data = output.GetBuffer();
                whole_buffer_size = data.size();
                bond::CompactBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_ints_deserialize(reader, msg_count);
                break;
            }
            case 3:
            {
                testcase_name = "mixed CompactBinary";
                serialize_time = test_serialize(writer, msg_count, generate_mixed);
                bond::blob data = output.GetBuffer();
                whole_buffer_size = data.size();
                bond::CompactBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_deserialize<mixed>(reader, msg_count);
                break;
            }
            case 4:
            {
                testcase_name = "container_vec CompactBinary";
                serialize_time = test_serialize(writer, msg_count, generate_contvec);
                bond::blob data = output.GetBuffer();
                whole_buffer_size = data.size();
                bond::CompactBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_deserialize<container_vec>(reader, msg_count);
                break;
            }
            case 5:
            {
                testcase_name = "container_map CompactBinary";
                serialize_time = test_serialize(writer, msg_count, generate_contmap);
                bond::blob data = output.GetBuffer();
                whole_buffer_size = data.size();
                bond::CompactBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_deserialize<container_map>(reader, msg_count);
                break;
            }
            case 6:
            {
                testcase_name = "complex CompactBinary";
                serialize_time = test_serialize<complex>(writer, msg_count, &generate_complex);
                bond::blob data = output.GetBuffer();
                whole_buffer_size = data.size();
                bond::CompactBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_deserialize<complex>(reader, msg_count);
                break;
            }
            case 11:
            {
                testcase_name = "ints-small FastBinary";
                serialize_time = test_ints_serialize(fast_writer, msg_count, small_int);
                bond::blob data = out_fastbin.GetBuffer();
                whole_buffer_size = data.size();
                bond::FastBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_ints_deserialize(reader, msg_count);
                break;
            }
            case 12:
            {
                testcase_name = "ints-large FastBinary";
                serialize_time = test_ints_serialize(fast_writer, msg_count, integer);
                bond::blob data = out_fastbin.GetBuffer();
                whole_buffer_size = data.size();
                bond::FastBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_ints_deserialize(reader, msg_count);
                break;
            }
            case 13:
            {
                testcase_name = "mixed FastBinary";
                serialize_time = test_serialize(fast_writer, msg_count, generate_mixed);
                bond::blob data = out_fastbin.GetBuffer();
                whole_buffer_size = data.size();
                bond::FastBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_deserialize<mixed>(reader, msg_count);
                break;
            }
            case 14:
            {
                testcase_name = "container_vec FastBinary";
                serialize_time = test_serialize(fast_writer, msg_count, generate_contvec);
                bond::blob data = out_fastbin.GetBuffer();
                whole_buffer_size = data.size();
                bond::FastBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_deserialize<container_vec>(reader, msg_count);
                break;
            }
            case 15:
            {
                testcase_name = "container_map FastBinary";
                serialize_time = test_serialize(fast_writer, msg_count, generate_contmap);
                bond::blob data = out_fastbin.GetBuffer();
                whole_buffer_size = data.size();
                bond::FastBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_deserialize<container_map>(reader, msg_count);
                break;
            }
            case 16:
            {
                testcase_name = "complex FastBinary";
                serialize_time = test_serialize<complex>(fast_writer, msg_count, &generate_complex);
                bond::blob data = out_fastbin.GetBuffer();
                whole_buffer_size = data.size();
                bond::FastBinaryReader<bond::InputBuffer> reader(data);
                deserialize_time = test_deserialize<complex>(reader, msg_count);
                break;
            }
            case 21:
            {
                testcase_name = "ints-small Json";
                serialize_time = test_ints_serialize(json_writer, msg_count, small_int);
                bond::blob data = out_json.GetBuffer();
                whole_buffer_size = data.size();
                bond::SimpleJsonReader<bond::InputBuffer> reader(data);
                deserialize_time = test_ints_deserialize(reader, msg_count);
                break;
            }
            case 22:
            {
                testcase_name = "ints-large Json";
                serialize_time = test_ints_serialize(json_writer, msg_count, integer);
                bond::blob data = out_json.GetBuffer();
                whole_buffer_size = data.size();
                bond::SimpleJsonReader<bond::InputBuffer> reader(data);
                deserialize_time = test_ints_deserialize(reader, msg_count);
                break;
            }
            case 23:
            {
                testcase_name = "mixed Json";
                serialize_time = test_mixed_serialize(json_writer, msg_count);
                bond::blob data = out_json.GetBuffer();
                whole_buffer_size = data.size();
                bond::SimpleJsonReader<bond::InputBuffer> reader(data);
                deserialize_time = test_mixed_deserialize(reader, msg_count);
                break;
            }
            case 24:
            {
                testcase_name = "container_vec Json";
                serialize_time = test_contvec_serialize(json_writer, msg_count);
                bond::blob data = out_json.GetBuffer();
                whole_buffer_size = data.size();
                bond::SimpleJsonReader<bond::InputBuffer> reader(data);
                deserialize_time = test_contvec_deserialize(reader, msg_count);
                break;
            }
            case 25:
            {
                testcase_name = "container_map Json";
                serialize_time = test_contmap_serialize(json_writer, msg_count);
                bond::blob data = out_json.GetBuffer();
                whole_buffer_size = data.size();
                bond::SimpleJsonReader<bond::InputBuffer> reader(data);
                deserialize_time = test_contmap_deserialize(reader, msg_count);
                break;
            }
            case 26:
            {
                testcase_name = "complex Json";
                serialize_time = test_serialize<complex>(json_writer, msg_count, &generate_complex);
                bond::blob data = out_json.GetBuffer();
                whole_buffer_size = data.size();
                bond::SimpleJsonReader<bond::InputBuffer> reader(data);
                deserialize_time = test_deserialize<complex>(reader, msg_count);
                break;
            }
            default:
                std::cout << "please provide a testcase number" << std::endl;
        }
        printStats(testcase_name, serialize_time, deserialize_time, msg_count, whole_buffer_size);
    }

    // appease compiler ...
    benchmark::State st(0, 0, 0, 0, 0, 0, 0);
    printSize(st, 0, 0);
    return 0;
}
