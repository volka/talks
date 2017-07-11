/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "serialize_types.h"

#include <algorithm>

namespace thrift_serialize {

int _kENUMERATIONValues[] = {
  ENUMERATION::ONE,
  ENUMERATION::TWO,
  ENUMERATION::THREE,
  ENUMERATION::FOUR
};
const char* _kENUMERATIONNames[] = {
  "ONE",
  "TWO",
  "THREE",
  "FOUR"
};
const std::map<int, const char*> _ENUMERATION_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(4, _kENUMERATIONValues, _kENUMERATIONNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

const char* ints::ascii_fingerprint = "3C83C6E0747D926A59C65E8FC8C39084";
const uint8_t ints::binary_fingerprint[16] = {0x3C,0x83,0xC6,0xE0,0x74,0x7D,0x92,0x6A,0x59,0xC6,0x5E,0x8F,0xC8,0xC3,0x90,0x84};

uint32_t ints::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->id);
          this->__isset.id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->int1);
          this->__isset.int1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->int2);
          this->__isset.int2 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->int3);
          this->__isset.int3 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->int4);
          this->__isset.int4 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t ints::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("ints");

  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int1", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->int1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int2", ::apache::thrift::protocol::T_I64, 3);
  xfer += oprot->writeI64(this->int2);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int3", ::apache::thrift::protocol::T_I32, 4);
  xfer += oprot->writeI32(this->int3);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int4", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32(this->int4);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ints &a, ints &b) {
  using ::std::swap;
  swap(a.id, b.id);
  swap(a.int1, b.int1);
  swap(a.int2, b.int2);
  swap(a.int3, b.int3);
  swap(a.int4, b.int4);
  swap(a.__isset, b.__isset);
}

const char* mixed::ascii_fingerprint = "93BB92AB0EC9C814FB55637E50B5208C";
const uint8_t mixed::binary_fingerprint[16] = {0x93,0xBB,0x92,0xAB,0x0E,0xC9,0xC8,0x14,0xFB,0x55,0x63,0x7E,0x50,0xB5,0x20,0x8C};

uint32_t mixed::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->id);
          this->__isset.id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->int1);
          this->__isset.int1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->int2);
          this->__isset.int2 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->int3);
          this->__isset.int3 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->float1);
          this->__isset.float1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->text1);
          this->__isset.text1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast0;
          xfer += iprot->readI32(ecast0);
          this->enum1 = (ENUMERATION::type)ecast0;
          this->__isset.enum1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t mixed::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("mixed");

  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int1", ::apache::thrift::protocol::T_I32, 2);
  xfer += oprot->writeI32(this->int1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int2", ::apache::thrift::protocol::T_I64, 3);
  xfer += oprot->writeI64(this->int2);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int3", ::apache::thrift::protocol::T_I64, 4);
  xfer += oprot->writeI64(this->int3);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("float1", ::apache::thrift::protocol::T_DOUBLE, 5);
  xfer += oprot->writeDouble(this->float1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("text1", ::apache::thrift::protocol::T_STRING, 6);
  xfer += oprot->writeString(this->text1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("enum1", ::apache::thrift::protocol::T_I32, 7);
  xfer += oprot->writeI32((int32_t)this->enum1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(mixed &a, mixed &b) {
  using ::std::swap;
  swap(a.id, b.id);
  swap(a.int1, b.int1);
  swap(a.int2, b.int2);
  swap(a.int3, b.int3);
  swap(a.float1, b.float1);
  swap(a.text1, b.text1);
  swap(a.enum1, b.enum1);
  swap(a.__isset, b.__isset);
}

const char* container_vec::ascii_fingerprint = "DD16BA0BB5133F139B056F1DCD6F058A";
const uint8_t container_vec::binary_fingerprint[16] = {0xDD,0x16,0xBA,0x0B,0xB5,0x13,0x3F,0x13,0x9B,0x05,0x6F,0x1D,0xCD,0x6F,0x05,0x8A};

uint32_t container_vec::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->id);
          this->__isset.id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->int1);
          this->__isset.int1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->text1);
          this->__isset.text1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->stringvec.clear();
            uint32_t _size1;
            ::apache::thrift::protocol::TType _etype4;
            xfer += iprot->readListBegin(_etype4, _size1);
            this->stringvec.resize(_size1);
            uint32_t _i5;
            for (_i5 = 0; _i5 < _size1; ++_i5)
            {
              xfer += iprot->readString(this->stringvec[_i5]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.stringvec = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->intvec.clear();
            uint32_t _size6;
            ::apache::thrift::protocol::TType _etype9;
            xfer += iprot->readListBegin(_etype9, _size6);
            this->intvec.resize(_size6);
            uint32_t _i10;
            for (_i10 = 0; _i10 < _size6; ++_i10)
            {
              xfer += iprot->readI64(this->intvec[_i10]);
            }
            xfer += iprot->readListEnd();
          }
          this->__isset.intvec = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t container_vec::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("container_vec");

  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int1", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->int1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("text1", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->text1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("stringvec", ::apache::thrift::protocol::T_LIST, 4);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->stringvec.size()));
    std::vector<std::string> ::const_iterator _iter11;
    for (_iter11 = this->stringvec.begin(); _iter11 != this->stringvec.end(); ++_iter11)
    {
      xfer += oprot->writeString((*_iter11));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("intvec", ::apache::thrift::protocol::T_LIST, 5);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I64, static_cast<uint32_t>(this->intvec.size()));
    std::vector<int64_t> ::const_iterator _iter12;
    for (_iter12 = this->intvec.begin(); _iter12 != this->intvec.end(); ++_iter12)
    {
      xfer += oprot->writeI64((*_iter12));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(container_vec &a, container_vec &b) {
  using ::std::swap;
  swap(a.id, b.id);
  swap(a.int1, b.int1);
  swap(a.text1, b.text1);
  swap(a.stringvec, b.stringvec);
  swap(a.intvec, b.intvec);
  swap(a.__isset, b.__isset);
}

const char* container_map::ascii_fingerprint = "1C8C5BD8F089C0949B57A595A630C52B";
const uint8_t container_map::binary_fingerprint[16] = {0x1C,0x8C,0x5B,0xD8,0xF0,0x89,0xC0,0x94,0x9B,0x57,0xA5,0x95,0xA6,0x30,0xC5,0x2B};

uint32_t container_map::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->id);
          this->__isset.id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->int1);
          this->__isset.int1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->text1);
          this->__isset.text1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_MAP) {
          {
            this->map1.clear();
            uint32_t _size13;
            ::apache::thrift::protocol::TType _ktype14;
            ::apache::thrift::protocol::TType _vtype15;
            xfer += iprot->readMapBegin(_ktype14, _vtype15, _size13);
            uint32_t _i17;
            for (_i17 = 0; _i17 < _size13; ++_i17)
            {
              int32_t _key18;
              xfer += iprot->readI32(_key18);
              std::string& _val19 = this->map1[_key18];
              xfer += iprot->readString(_val19);
            }
            xfer += iprot->readMapEnd();
          }
          this->__isset.map1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t container_map::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("container_map");

  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int1", ::apache::thrift::protocol::T_I64, 2);
  xfer += oprot->writeI64(this->int1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("text1", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->text1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("map1", ::apache::thrift::protocol::T_MAP, 4);
  {
    xfer += oprot->writeMapBegin(::apache::thrift::protocol::T_I32, ::apache::thrift::protocol::T_STRING, static_cast<uint32_t>(this->map1.size()));
    std::map<int32_t, std::string> ::const_iterator _iter20;
    for (_iter20 = this->map1.begin(); _iter20 != this->map1.end(); ++_iter20)
    {
      xfer += oprot->writeI32(_iter20->first);
      xfer += oprot->writeString(_iter20->second);
    }
    xfer += oprot->writeMapEnd();
  }
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(container_map &a, container_map &b) {
  using ::std::swap;
  swap(a.id, b.id);
  swap(a.int1, b.int1);
  swap(a.text1, b.text1);
  swap(a.map1, b.map1);
  swap(a.__isset, b.__isset);
}

const char* complex::ascii_fingerprint = "8610B2CE5872B1B4FDE38A16FA17BF4D";
const uint8_t complex::binary_fingerprint[16] = {0x86,0x10,0xB2,0xCE,0x58,0x72,0xB1,0xB4,0xFD,0xE3,0x8A,0x16,0xFA,0x17,0xBF,0x4D};

uint32_t complex::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->id);
          this->__isset.id = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->text1);
          this->__isset.text1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->text2);
          this->__isset.text2 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->text3);
          this->__isset.text3 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->int1);
          this->__isset.int1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 6:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->int2);
          this->__isset.int2 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 7:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->int3);
          this->__isset.int3 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 8:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->text4);
          this->__isset.text4 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 9:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          int32_t ecast21;
          xfer += iprot->readI32(ecast21);
          this->enum1 = (ENUMERATION::type)ecast21;
          this->__isset.enum1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 10:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->float1);
          this->__isset.float1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 11:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->int4);
          this->__isset.int4 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 12:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->uint1);
          this->__isset.uint1 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 13:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->uint2);
          this->__isset.uint2 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 14:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->int5);
          this->__isset.int5 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 15:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->int6);
          this->__isset.int6 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 16:
        if (ftype == ::apache::thrift::protocol::T_DOUBLE) {
          xfer += iprot->readDouble(this->float2);
          this->__isset.float2 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 17:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->text5);
          this->__isset.text5 = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t complex::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("complex");

  xfer += oprot->writeFieldBegin("id", ::apache::thrift::protocol::T_I64, 1);
  xfer += oprot->writeI64(this->id);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("text1", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeString(this->text1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("text2", ::apache::thrift::protocol::T_STRING, 3);
  xfer += oprot->writeString(this->text2);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("text3", ::apache::thrift::protocol::T_STRING, 4);
  xfer += oprot->writeString(this->text3);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int1", ::apache::thrift::protocol::T_I32, 5);
  xfer += oprot->writeI32(this->int1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int2", ::apache::thrift::protocol::T_I32, 6);
  xfer += oprot->writeI32(this->int2);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int3", ::apache::thrift::protocol::T_I32, 7);
  xfer += oprot->writeI32(this->int3);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("text4", ::apache::thrift::protocol::T_STRING, 8);
  xfer += oprot->writeString(this->text4);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("enum1", ::apache::thrift::protocol::T_I32, 9);
  xfer += oprot->writeI32((int32_t)this->enum1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("float1", ::apache::thrift::protocol::T_DOUBLE, 10);
  xfer += oprot->writeDouble(this->float1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int4", ::apache::thrift::protocol::T_I64, 11);
  xfer += oprot->writeI64(this->int4);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("uint1", ::apache::thrift::protocol::T_I32, 12);
  xfer += oprot->writeI32(this->uint1);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("uint2", ::apache::thrift::protocol::T_I64, 13);
  xfer += oprot->writeI64(this->uint2);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int5", ::apache::thrift::protocol::T_I64, 14);
  xfer += oprot->writeI64(this->int5);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("int6", ::apache::thrift::protocol::T_I32, 15);
  xfer += oprot->writeI32(this->int6);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("float2", ::apache::thrift::protocol::T_DOUBLE, 16);
  xfer += oprot->writeDouble(this->float2);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("text5", ::apache::thrift::protocol::T_STRING, 17);
  xfer += oprot->writeString(this->text5);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(complex &a, complex &b) {
  using ::std::swap;
  swap(a.id, b.id);
  swap(a.text1, b.text1);
  swap(a.text2, b.text2);
  swap(a.text3, b.text3);
  swap(a.int1, b.int1);
  swap(a.int2, b.int2);
  swap(a.int3, b.int3);
  swap(a.text4, b.text4);
  swap(a.enum1, b.enum1);
  swap(a.float1, b.float1);
  swap(a.int4, b.int4);
  swap(a.uint1, b.uint1);
  swap(a.uint2, b.uint2);
  swap(a.int5, b.int5);
  swap(a.int6, b.int6);
  swap(a.float2, b.float2);
  swap(a.text5, b.text5);
  swap(a.__isset, b.__isset);
}

const char* Message::ascii_fingerprint = "303024DB9EB3741B1EEC81F8AC9C6DC9";
const uint8_t Message::binary_fingerprint[16] = {0x30,0x30,0x24,0xDB,0x9E,0xB3,0x74,0x1B,0x1E,0xEC,0x81,0xF8,0xAC,0x9C,0x6D,0xC9};

uint32_t Message::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->int_msg.read(iprot);
          this->__isset.int_msg = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->mixed_msg.read(iprot);
          this->__isset.mixed_msg = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->container_vec_msg.read(iprot);
          this->__isset.container_vec_msg = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 4:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->container_map_msg.read(iprot);
          this->__isset.container_map_msg = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 5:
        if (ftype == ::apache::thrift::protocol::T_STRUCT) {
          xfer += this->complex_msg.read(iprot);
          this->__isset.complex_msg = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Message::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  xfer += oprot->writeStructBegin("Message");

  xfer += oprot->writeFieldBegin("int_msg", ::apache::thrift::protocol::T_STRUCT, 1);
  xfer += this->int_msg.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("mixed_msg", ::apache::thrift::protocol::T_STRUCT, 2);
  xfer += this->mixed_msg.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("container_vec_msg", ::apache::thrift::protocol::T_STRUCT, 3);
  xfer += this->container_vec_msg.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("container_map_msg", ::apache::thrift::protocol::T_STRUCT, 4);
  xfer += this->container_map_msg.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("complex_msg", ::apache::thrift::protocol::T_STRUCT, 5);
  xfer += this->complex_msg.write(oprot);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(Message &a, Message &b) {
  using ::std::swap;
  swap(a.int_msg, b.int_msg);
  swap(a.mixed_msg, b.mixed_msg);
  swap(a.container_vec_msg, b.container_vec_msg);
  swap(a.container_map_msg, b.container_map_msg);
  swap(a.complex_msg, b.complex_msg);
  swap(a.__isset, b.__isset);
}

} // namespace