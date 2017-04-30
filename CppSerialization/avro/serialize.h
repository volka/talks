/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef SERIALIZE_H_2612893099__H_
#define SERIALIZE_H_2612893099__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace avro_bench {
struct ints {
    int64_t id;
    int64_t int1;
    int64_t int2;
    int32_t int3;
    int32_t int4;
    ints() :
        id(int64_t()),
        int1(int64_t()),
        int2(int64_t()),
        int3(int32_t()),
        int4(int32_t())
        { }
};

struct serialize_avro_Union__0__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    std::vector<ints > get_array() const;
    void set_array(const std::vector<ints >& v);
    serialize_avro_Union__0__();
};

enum ENUM {
    ONE,
    TWO,
    THREE,
    FOUR,
};

struct mixed {
    int32_t id;
    int32_t int1;
    int64_t int2;
    int64_t uint1;
    double float1;
    std::string text1;
    ENUM enum1;
    mixed() :
        id(int32_t()),
        int1(int32_t()),
        int2(int64_t()),
        uint1(int64_t()),
        float1(double()),
        text1(std::string()),
        enum1(ENUM())
        { }
};

struct serialize_avro_Union__1__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    std::vector<mixed > get_array() const;
    void set_array(const std::vector<mixed >& v);
    serialize_avro_Union__1__();
};

struct complex {
    int64_t id;
    std::string text1;
    std::string text2;
    std::string text3;
    int32_t int1;
    int32_t int2;
    int32_t int3;
    std::string text4;
    ENUM enum1;
    double float1;
    int64_t int4;
    int64_t uint1;
    int64_t uint2;
    int64_t int5;
    int32_t int6;
    double float2;
    std::string text5;
    complex() :
        id(int64_t()),
        text1(std::string()),
        text2(std::string()),
        text3(std::string()),
        int1(int32_t()),
        int2(int32_t()),
        int3(int32_t()),
        text4(std::string()),
        enum1(ENUM()),
        float1(double()),
        int4(int64_t()),
        uint1(int64_t()),
        uint2(int64_t()),
        int5(int64_t()),
        int6(int32_t()),
        float2(double()),
        text5(std::string())
        { }
};

struct serialize_avro_Union__2__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    std::vector<complex > get_array() const;
    void set_array(const std::vector<complex >& v);
    serialize_avro_Union__2__();
};

struct container_vec {
    int32_t id;
    int64_t int1;
    std::string text1;
    std::vector<std::string > stringvec;
    std::vector<int64_t > intvec;
    container_vec() :
        id(int32_t()),
        int1(int64_t()),
        text1(std::string()),
        stringvec(std::vector<std::string >()),
        intvec(std::vector<int64_t >())
        { }
};

struct serialize_avro_Union__3__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    std::vector<container_vec > get_array() const;
    void set_array(const std::vector<container_vec >& v);
    serialize_avro_Union__3__();
};

struct container_map {
    int32_t id;
    int64_t int1;
    std::string text1;
    std::map<std::string, std::string > map1;
    container_map() :
        id(int32_t()),
        int1(int64_t()),
        text1(std::string()),
        map1(std::map<std::string, std::string >())
        { }
};

struct serialize_avro_Union__4__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    std::vector<container_map > get_array() const;
    void set_array(const std::vector<container_map >& v);
    serialize_avro_Union__4__();
};

struct root {
    typedef serialize_avro_Union__0__ ints_vec_t;
    typedef serialize_avro_Union__1__ mixed_vec_t;
    typedef serialize_avro_Union__2__ complex_vec_t;
    typedef serialize_avro_Union__3__ cont_vec_vec_t;
    typedef serialize_avro_Union__4__ cont_map_vec_t;
    ints_vec_t ints_vec;
    mixed_vec_t mixed_vec;
    complex_vec_t complex_vec;
    cont_vec_vec_t cont_vec_vec;
    cont_map_vec_t cont_map_vec;
    root() :
        ints_vec(ints_vec_t()),
        mixed_vec(mixed_vec_t()),
        complex_vec(complex_vec_t()),
        cont_vec_vec(cont_vec_vec_t()),
        cont_map_vec(cont_map_vec_t())
        { }
};

inline
std::vector<ints > serialize_avro_Union__0__::get_array() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::vector<ints > >(value_);
}

inline
void serialize_avro_Union__0__::set_array(const std::vector<ints >& v) {
    idx_ = 1;
    value_ = v;
}

inline
std::vector<mixed > serialize_avro_Union__1__::get_array() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::vector<mixed > >(value_);
}

inline
void serialize_avro_Union__1__::set_array(const std::vector<mixed >& v) {
    idx_ = 1;
    value_ = v;
}

inline
std::vector<complex > serialize_avro_Union__2__::get_array() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::vector<complex > >(value_);
}

inline
void serialize_avro_Union__2__::set_array(const std::vector<complex >& v) {
    idx_ = 1;
    value_ = v;
}

inline
std::vector<container_vec > serialize_avro_Union__3__::get_array() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::vector<container_vec > >(value_);
}

inline
void serialize_avro_Union__3__::set_array(const std::vector<container_vec >& v) {
    idx_ = 1;
    value_ = v;
}

inline
std::vector<container_map > serialize_avro_Union__4__::get_array() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<std::vector<container_map > >(value_);
}

inline
void serialize_avro_Union__4__::set_array(const std::vector<container_map >& v) {
    idx_ = 1;
    value_ = v;
}

inline serialize_avro_Union__0__::serialize_avro_Union__0__() : idx_(0) { }
inline serialize_avro_Union__1__::serialize_avro_Union__1__() : idx_(0) { }
inline serialize_avro_Union__2__::serialize_avro_Union__2__() : idx_(0) { }
inline serialize_avro_Union__3__::serialize_avro_Union__3__() : idx_(0) { }
inline serialize_avro_Union__4__::serialize_avro_Union__4__() : idx_(0) { }
}
namespace avro {
template<> struct codec_traits<avro_bench::ints> {
    static void encode(Encoder& e, const avro_bench::ints& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.int1);
        avro::encode(e, v.int2);
        avro::encode(e, v.int3);
        avro::encode(e, v.int4);
    }
    static void decode(Decoder& d, avro_bench::ints& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.int1);
                    break;
                case 2:
                    avro::decode(d, v.int2);
                    break;
                case 3:
                    avro::decode(d, v.int3);
                    break;
                case 4:
                    avro::decode(d, v.int4);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.int1);
            avro::decode(d, v.int2);
            avro::decode(d, v.int3);
            avro::decode(d, v.int4);
        }
    }
};

template<> struct codec_traits<avro_bench::serialize_avro_Union__0__> {
    static void encode(Encoder& e, avro_bench::serialize_avro_Union__0__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_array());
            break;
        }
    }
    static void decode(Decoder& d, avro_bench::serialize_avro_Union__0__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                std::vector<avro_bench::ints > vv;
                avro::decode(d, vv);
                v.set_array(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<avro_bench::ENUM> {
    static void encode(Encoder& e, avro_bench::ENUM v) {
		if (v < avro_bench::ONE || v > avro_bench::FOUR)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for avro_bench::ENUM and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, avro_bench::ENUM& v) {
		size_t index = d.decodeEnum();
		if (index < avro_bench::ONE || index > avro_bench::FOUR)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for avro_bench::ENUM and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<avro_bench::ENUM>(index);
    }
};

template<> struct codec_traits<avro_bench::mixed> {
    static void encode(Encoder& e, const avro_bench::mixed& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.int1);
        avro::encode(e, v.int2);
        avro::encode(e, v.uint1);
        avro::encode(e, v.float1);
        avro::encode(e, v.text1);
        avro::encode(e, v.enum1);
    }
    static void decode(Decoder& d, avro_bench::mixed& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.int1);
                    break;
                case 2:
                    avro::decode(d, v.int2);
                    break;
                case 3:
                    avro::decode(d, v.uint1);
                    break;
                case 4:
                    avro::decode(d, v.float1);
                    break;
                case 5:
                    avro::decode(d, v.text1);
                    break;
                case 6:
                    avro::decode(d, v.enum1);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.int1);
            avro::decode(d, v.int2);
            avro::decode(d, v.uint1);
            avro::decode(d, v.float1);
            avro::decode(d, v.text1);
            avro::decode(d, v.enum1);
        }
    }
};

template<> struct codec_traits<avro_bench::serialize_avro_Union__1__> {
    static void encode(Encoder& e, avro_bench::serialize_avro_Union__1__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_array());
            break;
        }
    }
    static void decode(Decoder& d, avro_bench::serialize_avro_Union__1__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                std::vector<avro_bench::mixed > vv;
                avro::decode(d, vv);
                v.set_array(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<avro_bench::complex> {
    static void encode(Encoder& e, const avro_bench::complex& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.text1);
        avro::encode(e, v.text2);
        avro::encode(e, v.text3);
        avro::encode(e, v.int1);
        avro::encode(e, v.int2);
        avro::encode(e, v.int3);
        avro::encode(e, v.text4);
        avro::encode(e, v.enum1);
        avro::encode(e, v.float1);
        avro::encode(e, v.int4);
        avro::encode(e, v.uint1);
        avro::encode(e, v.uint2);
        avro::encode(e, v.int5);
        avro::encode(e, v.int6);
        avro::encode(e, v.float2);
        avro::encode(e, v.text5);
    }
    static void decode(Decoder& d, avro_bench::complex& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.text1);
                    break;
                case 2:
                    avro::decode(d, v.text2);
                    break;
                case 3:
                    avro::decode(d, v.text3);
                    break;
                case 4:
                    avro::decode(d, v.int1);
                    break;
                case 5:
                    avro::decode(d, v.int2);
                    break;
                case 6:
                    avro::decode(d, v.int3);
                    break;
                case 7:
                    avro::decode(d, v.text4);
                    break;
                case 8:
                    avro::decode(d, v.enum1);
                    break;
                case 9:
                    avro::decode(d, v.float1);
                    break;
                case 10:
                    avro::decode(d, v.int4);
                    break;
                case 11:
                    avro::decode(d, v.uint1);
                    break;
                case 12:
                    avro::decode(d, v.uint2);
                    break;
                case 13:
                    avro::decode(d, v.int5);
                    break;
                case 14:
                    avro::decode(d, v.int6);
                    break;
                case 15:
                    avro::decode(d, v.float2);
                    break;
                case 16:
                    avro::decode(d, v.text5);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.text1);
            avro::decode(d, v.text2);
            avro::decode(d, v.text3);
            avro::decode(d, v.int1);
            avro::decode(d, v.int2);
            avro::decode(d, v.int3);
            avro::decode(d, v.text4);
            avro::decode(d, v.enum1);
            avro::decode(d, v.float1);
            avro::decode(d, v.int4);
            avro::decode(d, v.uint1);
            avro::decode(d, v.uint2);
            avro::decode(d, v.int5);
            avro::decode(d, v.int6);
            avro::decode(d, v.float2);
            avro::decode(d, v.text5);
        }
    }
};

template<> struct codec_traits<avro_bench::serialize_avro_Union__2__> {
    static void encode(Encoder& e, avro_bench::serialize_avro_Union__2__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_array());
            break;
        }
    }
    static void decode(Decoder& d, avro_bench::serialize_avro_Union__2__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                std::vector<avro_bench::complex > vv;
                avro::decode(d, vv);
                v.set_array(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<avro_bench::container_vec> {
    static void encode(Encoder& e, const avro_bench::container_vec& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.int1);
        avro::encode(e, v.text1);
        avro::encode(e, v.stringvec);
        avro::encode(e, v.intvec);
    }
    static void decode(Decoder& d, avro_bench::container_vec& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.int1);
                    break;
                case 2:
                    avro::decode(d, v.text1);
                    break;
                case 3:
                    avro::decode(d, v.stringvec);
                    break;
                case 4:
                    avro::decode(d, v.intvec);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.int1);
            avro::decode(d, v.text1);
            avro::decode(d, v.stringvec);
            avro::decode(d, v.intvec);
        }
    }
};

template<> struct codec_traits<avro_bench::serialize_avro_Union__3__> {
    static void encode(Encoder& e, avro_bench::serialize_avro_Union__3__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_array());
            break;
        }
    }
    static void decode(Decoder& d, avro_bench::serialize_avro_Union__3__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                std::vector<avro_bench::container_vec > vv;
                avro::decode(d, vv);
                v.set_array(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<avro_bench::container_map> {
    static void encode(Encoder& e, const avro_bench::container_map& v) {
        avro::encode(e, v.id);
        avro::encode(e, v.int1);
        avro::encode(e, v.text1);
        avro::encode(e, v.map1);
    }
    static void decode(Decoder& d, avro_bench::container_map& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.id);
                    break;
                case 1:
                    avro::decode(d, v.int1);
                    break;
                case 2:
                    avro::decode(d, v.text1);
                    break;
                case 3:
                    avro::decode(d, v.map1);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.id);
            avro::decode(d, v.int1);
            avro::decode(d, v.text1);
            avro::decode(d, v.map1);
        }
    }
};

template<> struct codec_traits<avro_bench::serialize_avro_Union__4__> {
    static void encode(Encoder& e, avro_bench::serialize_avro_Union__4__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_array());
            break;
        }
    }
    static void decode(Decoder& d, avro_bench::serialize_avro_Union__4__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                std::vector<avro_bench::container_map > vv;
                avro::decode(d, vv);
                v.set_array(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<avro_bench::root> {
    static void encode(Encoder& e, const avro_bench::root& v) {
        avro::encode(e, v.ints_vec);
        avro::encode(e, v.mixed_vec);
        avro::encode(e, v.complex_vec);
        avro::encode(e, v.cont_vec_vec);
        avro::encode(e, v.cont_map_vec);
    }
    static void decode(Decoder& d, avro_bench::root& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.ints_vec);
                    break;
                case 1:
                    avro::decode(d, v.mixed_vec);
                    break;
                case 2:
                    avro::decode(d, v.complex_vec);
                    break;
                case 3:
                    avro::decode(d, v.cont_vec_vec);
                    break;
                case 4:
                    avro::decode(d, v.cont_map_vec);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.ints_vec);
            avro::decode(d, v.mixed_vec);
            avro::decode(d, v.complex_vec);
            avro::decode(d, v.cont_vec_vec);
            avro::decode(d, v.cont_map_vec);
        }
    }
};

}
#endif
