namespace cpp thrift_serialize

enum ENUMERATION {
    ONE = 0,
    TWO = 1,
    THREE = 2,
    FOUR = 3
}

struct ints {
    1: i64 id,
    2: i64 int1,
    3: i64 int2,
    4: i32 int3,
    5: i32 int4
}

struct mixed {
    1: i32 id,
    2: i32 int1,
    3: i64 int2,
    4: i64 int3, // no uint type, so use i64 instead
    5: double float1, 
    6: string text1,
    7: ENUMERATION enum1
}

struct container_vec {
    1: i32 id,
    2: i64 int1,
    3: string text1,
    4: list<string> stringvec,
    5: list<i64> intvec
}

struct container_map {
    1: i32 id,
    2: i64 int1,
    3: string text1,
    4: map<i32, string> map1
}

struct complex {
    1: i64 id,
    2: string text1,
    3: string text2,
    4: string text3,
    5: i32 int1,
    6: i32 int2,
    7: i32 int3,
    8: string text4,
    9: ENUMERATION enum1,
    10: double float1,
    11: i64 int4,
    12: i32 uint1,
    13: i64 uint2,
    14: i64 int5,
    15: i32 int6,
    16: double float2,
    17: string text5
}

union Message {
    1: ints int_msg;
    2: mixed mixed_msg;
    3: container_vec container_vec_msg;
    4: container_map container_map_msg;
    5: complex complex_msg;
}
