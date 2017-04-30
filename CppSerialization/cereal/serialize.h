#ifndef CEREAL_SERIALIZE_BENCH
#define CEREAL_SERIALIZE_BENCH

#include "datasets.h"

// note: we use BOOST_SERIALIZATION_NVP here but set it to a NOOP for the
// Cereal test, so we can reuse this file for Boost and Cereal

namespace cereal_bench {

struct Ints : public ints_t
{
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* version */)
    {
        ar & BOOST_SERIALIZATION_NVP(id);
        ar & BOOST_SERIALIZATION_NVP(int1);
        ar & BOOST_SERIALIZATION_NVP(int2);
        ar & BOOST_SERIALIZATION_NVP(int3);
        ar & BOOST_SERIALIZATION_NVP(int4);
    }
};

struct Mixed : public mixed_t
{
    template <class Archive>
    void serialize(Archive& ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_NVP(id);
        ar & BOOST_SERIALIZATION_NVP(int1);
        ar & BOOST_SERIALIZATION_NVP(int2);
        ar & BOOST_SERIALIZATION_NVP(uint1);
        ar & BOOST_SERIALIZATION_NVP(float1);
        ar & BOOST_SERIALIZATION_NVP(text1);
        ar & BOOST_SERIALIZATION_NVP(enum1);
    }
};

struct Complex : public complex_t
{
    template <class Archive>
    void serialize(Archive& ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_NVP(id);
        ar & BOOST_SERIALIZATION_NVP(text1);
        ar & BOOST_SERIALIZATION_NVP(text2);
        ar & BOOST_SERIALIZATION_NVP(text3);
        ar & BOOST_SERIALIZATION_NVP(int1);
        ar & BOOST_SERIALIZATION_NVP(int2);
        ar & BOOST_SERIALIZATION_NVP(int3);
        ar & BOOST_SERIALIZATION_NVP(text4);
        ar & BOOST_SERIALIZATION_NVP(enum1);
        ar & BOOST_SERIALIZATION_NVP(float1);
        ar & BOOST_SERIALIZATION_NVP(int4);
        ar & BOOST_SERIALIZATION_NVP(uint1);
        ar & BOOST_SERIALIZATION_NVP(uint2);
        ar & BOOST_SERIALIZATION_NVP(int5);
        ar & BOOST_SERIALIZATION_NVP(int6);
        ar & BOOST_SERIALIZATION_NVP(float2);
        ar & BOOST_SERIALIZATION_NVP(text5);
    }
};

struct Container_Vec : public container_vec_t
{
    template <class Archive>
    void serialize(Archive& ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_NVP(id);
        ar & BOOST_SERIALIZATION_NVP(int1);
        ar & BOOST_SERIALIZATION_NVP(text1);
        ar & BOOST_SERIALIZATION_NVP(stringvec);
        ar & BOOST_SERIALIZATION_NVP(intvec);
    }
};

struct Container_Map : public container_map_t
{
    template <class Archive>
    void serialize(Archive& ar, const unsigned int)
    {
        ar & BOOST_SERIALIZATION_NVP(id);
        ar & BOOST_SERIALIZATION_NVP(int1);
        ar & BOOST_SERIALIZATION_NVP(text1);
        ar & BOOST_SERIALIZATION_NVP(map1);
    }
};

} // ns cereal_bench
#endif
