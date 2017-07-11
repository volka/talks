// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_SERIALIZE_FLATBUF_BENCH_H_
#define FLATBUFFERS_GENERATED_SERIALIZE_FLATBUF_BENCH_H_

#include "flatbuffers/flatbuffers.h"


namespace flatbuf_bench {

struct Root;
struct Ints;
struct Mixed;
struct Complex;
struct ContainerVec;
struct IntStringMap;
struct ContainerMap;

enum ENUMERATION {
  ENUMERATION_ONE = 0,
  ENUMERATION_TWO = 1,
  ENUMERATION_THREE = 2,
  ENUMERATION_FOUR = 3,
  ENUMERATION_MIN = ENUMERATION_ONE,
  ENUMERATION_MAX = ENUMERATION_FOUR
};

inline const char **EnumNamesENUMERATION() {
  static const char *names[] = { "ONE", "TWO", "THREE", "FOUR", nullptr };
  return names;
}

inline const char *EnumNameENUMERATION(ENUMERATION e) { return EnumNamesENUMERATION()[static_cast<int>(e)]; }

MANUALLY_ALIGNED_STRUCT(8) Ints FLATBUFFERS_FINAL_CLASS {
 private:
  int64_t id_;
  int64_t int1_;
  int64_t int2_;
  int32_t int3_;
  int32_t int4_;

 public:
  Ints(int64_t _id, int64_t _int1, int64_t _int2, int32_t _int3, int32_t _int4)
    : id_(flatbuffers::EndianScalar(_id)), int1_(flatbuffers::EndianScalar(_int1)), int2_(flatbuffers::EndianScalar(_int2)), int3_(flatbuffers::EndianScalar(_int3)), int4_(flatbuffers::EndianScalar(_int4)) { }

  int64_t id() const { return flatbuffers::EndianScalar(id_); }
  int64_t int1() const { return flatbuffers::EndianScalar(int1_); }
  int64_t int2() const { return flatbuffers::EndianScalar(int2_); }
  int32_t int3() const { return flatbuffers::EndianScalar(int3_); }
  int32_t int4() const { return flatbuffers::EndianScalar(int4_); }
};
STRUCT_END(Ints, 32);

struct Root FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_INTS = 4,
    VT_MIXED = 6,
    VT_COMPLEX = 8,
    VT_CVEC = 10,
    VT_CMAP = 12
  };
  const flatbuffers::Vector<const Ints *> *ints() const { return GetPointer<const flatbuffers::Vector<const Ints *> *>(VT_INTS); }
  const flatbuffers::Vector<flatbuffers::Offset<Mixed>> *mixed() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Mixed>> *>(VT_MIXED); }
  const flatbuffers::Vector<flatbuffers::Offset<Complex>> *complex() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Complex>> *>(VT_COMPLEX); }
  const flatbuffers::Vector<flatbuffers::Offset<ContainerVec>> *cvec() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<ContainerVec>> *>(VT_CVEC); }
  const flatbuffers::Vector<flatbuffers::Offset<ContainerMap>> *cmap() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<ContainerMap>> *>(VT_CMAP); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_INTS) &&
           verifier.Verify(ints()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_MIXED) &&
           verifier.Verify(mixed()) &&
           verifier.VerifyVectorOfTables(mixed()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_COMPLEX) &&
           verifier.Verify(complex()) &&
           verifier.VerifyVectorOfTables(complex()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_CVEC) &&
           verifier.Verify(cvec()) &&
           verifier.VerifyVectorOfTables(cvec()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_CMAP) &&
           verifier.Verify(cmap()) &&
           verifier.VerifyVectorOfTables(cmap()) &&
           verifier.EndTable();
  }
};

struct RootBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_ints(flatbuffers::Offset<flatbuffers::Vector<const Ints *>> ints) { fbb_.AddOffset(Root::VT_INTS, ints); }
  void add_mixed(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Mixed>>> mixed) { fbb_.AddOffset(Root::VT_MIXED, mixed); }
  void add_complex(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Complex>>> complex) { fbb_.AddOffset(Root::VT_COMPLEX, complex); }
  void add_cvec(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ContainerVec>>> cvec) { fbb_.AddOffset(Root::VT_CVEC, cvec); }
  void add_cmap(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ContainerMap>>> cmap) { fbb_.AddOffset(Root::VT_CMAP, cmap); }
  RootBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  RootBuilder &operator=(const RootBuilder &);
  flatbuffers::Offset<Root> Finish() {
    auto o = flatbuffers::Offset<Root>(fbb_.EndTable(start_, 5));
    return o;
  }
};

inline flatbuffers::Offset<Root> CreateRoot(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::Vector<const Ints *>> ints = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Mixed>>> mixed = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Complex>>> complex = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ContainerVec>>> cvec = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ContainerMap>>> cmap = 0) {
  RootBuilder builder_(_fbb);
  builder_.add_cmap(cmap);
  builder_.add_cvec(cvec);
  builder_.add_complex(complex);
  builder_.add_mixed(mixed);
  builder_.add_ints(ints);
  return builder_.Finish();
}

struct Mixed FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_INT1 = 6,
    VT_INT2 = 8,
    VT_UINT1 = 10,
    VT_FLOAT1 = 12,
    VT_TEXT1 = 14,
    VT_ENUM1 = 16
  };
  int32_t id() const { return GetField<int32_t>(VT_ID, 0); }
  int32_t int1() const { return GetField<int32_t>(VT_INT1, 0); }
  int64_t int2() const { return GetField<int64_t>(VT_INT2, 0); }
  uint32_t uint1() const { return GetField<uint32_t>(VT_UINT1, 0); }
  double float1() const { return GetField<double>(VT_FLOAT1, 0); }
  const flatbuffers::String *text1() const { return GetPointer<const flatbuffers::String *>(VT_TEXT1); }
  ENUMERATION enum1() const { return static_cast<ENUMERATION>(GetField<int8_t>(VT_ENUM1, 0)); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<int32_t>(verifier, VT_INT1) &&
           VerifyField<int64_t>(verifier, VT_INT2) &&
           VerifyField<uint32_t>(verifier, VT_UINT1) &&
           VerifyField<double>(verifier, VT_FLOAT1) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TEXT1) &&
           verifier.Verify(text1()) &&
           VerifyField<int8_t>(verifier, VT_ENUM1) &&
           verifier.EndTable();
  }
};

struct MixedBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) { fbb_.AddElement<int32_t>(Mixed::VT_ID, id, 0); }
  void add_int1(int32_t int1) { fbb_.AddElement<int32_t>(Mixed::VT_INT1, int1, 0); }
  void add_int2(int64_t int2) { fbb_.AddElement<int64_t>(Mixed::VT_INT2, int2, 0); }
  void add_uint1(uint32_t uint1) { fbb_.AddElement<uint32_t>(Mixed::VT_UINT1, uint1, 0); }
  void add_float1(double float1) { fbb_.AddElement<double>(Mixed::VT_FLOAT1, float1, 0); }
  void add_text1(flatbuffers::Offset<flatbuffers::String> text1) { fbb_.AddOffset(Mixed::VT_TEXT1, text1); }
  void add_enum1(ENUMERATION enum1) { fbb_.AddElement<int8_t>(Mixed::VT_ENUM1, static_cast<int8_t>(enum1), 0); }
  MixedBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  MixedBuilder &operator=(const MixedBuilder &);
  flatbuffers::Offset<Mixed> Finish() {
    auto o = flatbuffers::Offset<Mixed>(fbb_.EndTable(start_, 7));
    return o;
  }
};

inline flatbuffers::Offset<Mixed> CreateMixed(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t id = 0,
   int32_t int1 = 0,
   int64_t int2 = 0,
   uint32_t uint1 = 0,
   double float1 = 0,
   flatbuffers::Offset<flatbuffers::String> text1 = 0,
   ENUMERATION enum1 = ENUMERATION_ONE) {
  MixedBuilder builder_(_fbb);
  builder_.add_float1(float1);
  builder_.add_int2(int2);
  builder_.add_text1(text1);
  builder_.add_uint1(uint1);
  builder_.add_int1(int1);
  builder_.add_id(id);
  builder_.add_enum1(enum1);
  return builder_.Finish();
}

struct Complex FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_TEXT1 = 6,
    VT_TEXT2 = 8,
    VT_TEXT3 = 10,
    VT_INT1 = 12,
    VT_INT2 = 14,
    VT_INT3 = 16,
    VT_TEXT4 = 18,
    VT_ENUM1 = 20,
    VT_FLOAT1 = 22,
    VT_INT4 = 24,
    VT_UINT1 = 26,
    VT_UINT2 = 28,
    VT_INT5 = 30,
    VT_INT6 = 32,
    VT_FLOAT2 = 34,
    VT_TEXT5 = 36
  };
  int32_t id() const { return GetField<int32_t>(VT_ID, 0); }
  const flatbuffers::String *text1() const { return GetPointer<const flatbuffers::String *>(VT_TEXT1); }
  const flatbuffers::String *text2() const { return GetPointer<const flatbuffers::String *>(VT_TEXT2); }
  const flatbuffers::String *text3() const { return GetPointer<const flatbuffers::String *>(VT_TEXT3); }
  int32_t int1() const { return GetField<int32_t>(VT_INT1, 0); }
  int32_t int2() const { return GetField<int32_t>(VT_INT2, 0); }
  int32_t int3() const { return GetField<int32_t>(VT_INT3, 0); }
  const flatbuffers::String *text4() const { return GetPointer<const flatbuffers::String *>(VT_TEXT4); }
  ENUMERATION enum1() const { return static_cast<ENUMERATION>(GetField<int8_t>(VT_ENUM1, 0)); }
  double float1() const { return GetField<double>(VT_FLOAT1, 0); }
  int64_t int4() const { return GetField<int64_t>(VT_INT4, 0); }
  uint32_t uint1() const { return GetField<uint32_t>(VT_UINT1, 0); }
  uint64_t uint2() const { return GetField<uint64_t>(VT_UINT2, 0); }
  int64_t int5() const { return GetField<int64_t>(VT_INT5, 0); }
  int32_t int6() const { return GetField<int32_t>(VT_INT6, 0); }
  double float2() const { return GetField<double>(VT_FLOAT2, 0); }
  const flatbuffers::String *text5() const { return GetPointer<const flatbuffers::String *>(VT_TEXT5); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TEXT1) &&
           verifier.Verify(text1()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TEXT2) &&
           verifier.Verify(text2()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TEXT3) &&
           verifier.Verify(text3()) &&
           VerifyField<int32_t>(verifier, VT_INT1) &&
           VerifyField<int32_t>(verifier, VT_INT2) &&
           VerifyField<int32_t>(verifier, VT_INT3) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TEXT4) &&
           verifier.Verify(text4()) &&
           VerifyField<int8_t>(verifier, VT_ENUM1) &&
           VerifyField<double>(verifier, VT_FLOAT1) &&
           VerifyField<int64_t>(verifier, VT_INT4) &&
           VerifyField<uint32_t>(verifier, VT_UINT1) &&
           VerifyField<uint64_t>(verifier, VT_UINT2) &&
           VerifyField<int64_t>(verifier, VT_INT5) &&
           VerifyField<int32_t>(verifier, VT_INT6) &&
           VerifyField<double>(verifier, VT_FLOAT2) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TEXT5) &&
           verifier.Verify(text5()) &&
           verifier.EndTable();
  }
};

struct ComplexBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) { fbb_.AddElement<int32_t>(Complex::VT_ID, id, 0); }
  void add_text1(flatbuffers::Offset<flatbuffers::String> text1) { fbb_.AddOffset(Complex::VT_TEXT1, text1); }
  void add_text2(flatbuffers::Offset<flatbuffers::String> text2) { fbb_.AddOffset(Complex::VT_TEXT2, text2); }
  void add_text3(flatbuffers::Offset<flatbuffers::String> text3) { fbb_.AddOffset(Complex::VT_TEXT3, text3); }
  void add_int1(int32_t int1) { fbb_.AddElement<int32_t>(Complex::VT_INT1, int1, 0); }
  void add_int2(int32_t int2) { fbb_.AddElement<int32_t>(Complex::VT_INT2, int2, 0); }
  void add_int3(int32_t int3) { fbb_.AddElement<int32_t>(Complex::VT_INT3, int3, 0); }
  void add_text4(flatbuffers::Offset<flatbuffers::String> text4) { fbb_.AddOffset(Complex::VT_TEXT4, text4); }
  void add_enum1(ENUMERATION enum1) { fbb_.AddElement<int8_t>(Complex::VT_ENUM1, static_cast<int8_t>(enum1), 0); }
  void add_float1(double float1) { fbb_.AddElement<double>(Complex::VT_FLOAT1, float1, 0); }
  void add_int4(int64_t int4) { fbb_.AddElement<int64_t>(Complex::VT_INT4, int4, 0); }
  void add_uint1(uint32_t uint1) { fbb_.AddElement<uint32_t>(Complex::VT_UINT1, uint1, 0); }
  void add_uint2(uint64_t uint2) { fbb_.AddElement<uint64_t>(Complex::VT_UINT2, uint2, 0); }
  void add_int5(int64_t int5) { fbb_.AddElement<int64_t>(Complex::VT_INT5, int5, 0); }
  void add_int6(int32_t int6) { fbb_.AddElement<int32_t>(Complex::VT_INT6, int6, 0); }
  void add_float2(double float2) { fbb_.AddElement<double>(Complex::VT_FLOAT2, float2, 0); }
  void add_text5(flatbuffers::Offset<flatbuffers::String> text5) { fbb_.AddOffset(Complex::VT_TEXT5, text5); }
  ComplexBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  ComplexBuilder &operator=(const ComplexBuilder &);
  flatbuffers::Offset<Complex> Finish() {
    auto o = flatbuffers::Offset<Complex>(fbb_.EndTable(start_, 17));
    return o;
  }
};

inline flatbuffers::Offset<Complex> CreateComplex(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t id = 0,
   flatbuffers::Offset<flatbuffers::String> text1 = 0,
   flatbuffers::Offset<flatbuffers::String> text2 = 0,
   flatbuffers::Offset<flatbuffers::String> text3 = 0,
   int32_t int1 = 0,
   int32_t int2 = 0,
   int32_t int3 = 0,
   flatbuffers::Offset<flatbuffers::String> text4 = 0,
   ENUMERATION enum1 = ENUMERATION_ONE,
   double float1 = 0,
   int64_t int4 = 0,
   uint32_t uint1 = 0,
   uint64_t uint2 = 0,
   int64_t int5 = 0,
   int32_t int6 = 0,
   double float2 = 0,
   flatbuffers::Offset<flatbuffers::String> text5 = 0) {
  ComplexBuilder builder_(_fbb);
  builder_.add_float2(float2);
  builder_.add_int5(int5);
  builder_.add_uint2(uint2);
  builder_.add_int4(int4);
  builder_.add_float1(float1);
  builder_.add_text5(text5);
  builder_.add_int6(int6);
  builder_.add_uint1(uint1);
  builder_.add_text4(text4);
  builder_.add_int3(int3);
  builder_.add_int2(int2);
  builder_.add_int1(int1);
  builder_.add_text3(text3);
  builder_.add_text2(text2);
  builder_.add_text1(text1);
  builder_.add_id(id);
  builder_.add_enum1(enum1);
  return builder_.Finish();
}

struct ContainerVec FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_INT1 = 6,
    VT_TEXT1 = 8,
    VT_STRINGVEC = 10,
    VT_INTVEC = 12
  };
  int32_t id() const { return GetField<int32_t>(VT_ID, 0); }
  int64_t int1() const { return GetField<int64_t>(VT_INT1, 0); }
  const flatbuffers::String *text1() const { return GetPointer<const flatbuffers::String *>(VT_TEXT1); }
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *stringvec() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_STRINGVEC); }
  const flatbuffers::Vector<int64_t> *intvec() const { return GetPointer<const flatbuffers::Vector<int64_t> *>(VT_INTVEC); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<int64_t>(verifier, VT_INT1) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TEXT1) &&
           verifier.Verify(text1()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_STRINGVEC) &&
           verifier.Verify(stringvec()) &&
           verifier.VerifyVectorOfStrings(stringvec()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_INTVEC) &&
           verifier.Verify(intvec()) &&
           verifier.EndTable();
  }
};

struct ContainerVecBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) { fbb_.AddElement<int32_t>(ContainerVec::VT_ID, id, 0); }
  void add_int1(int64_t int1) { fbb_.AddElement<int64_t>(ContainerVec::VT_INT1, int1, 0); }
  void add_text1(flatbuffers::Offset<flatbuffers::String> text1) { fbb_.AddOffset(ContainerVec::VT_TEXT1, text1); }
  void add_stringvec(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> stringvec) { fbb_.AddOffset(ContainerVec::VT_STRINGVEC, stringvec); }
  void add_intvec(flatbuffers::Offset<flatbuffers::Vector<int64_t>> intvec) { fbb_.AddOffset(ContainerVec::VT_INTVEC, intvec); }
  ContainerVecBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  ContainerVecBuilder &operator=(const ContainerVecBuilder &);
  flatbuffers::Offset<ContainerVec> Finish() {
    auto o = flatbuffers::Offset<ContainerVec>(fbb_.EndTable(start_, 5));
    return o;
  }
};

inline flatbuffers::Offset<ContainerVec> CreateContainerVec(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t id = 0,
   int64_t int1 = 0,
   flatbuffers::Offset<flatbuffers::String> text1 = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> stringvec = 0,
   flatbuffers::Offset<flatbuffers::Vector<int64_t>> intvec = 0) {
  ContainerVecBuilder builder_(_fbb);
  builder_.add_int1(int1);
  builder_.add_intvec(intvec);
  builder_.add_stringvec(stringvec);
  builder_.add_text1(text1);
  builder_.add_id(id);
  return builder_.Finish();
}

struct IntStringMap FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_VALUE = 6
  };
  int32_t id() const { return GetField<int32_t>(VT_ID, 0); }
  const flatbuffers::String *value() const { return GetPointer<const flatbuffers::String *>(VT_VALUE); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_VALUE) &&
           verifier.Verify(value()) &&
           verifier.EndTable();
  }
};

struct IntStringMapBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) { fbb_.AddElement<int32_t>(IntStringMap::VT_ID, id, 0); }
  void add_value(flatbuffers::Offset<flatbuffers::String> value) { fbb_.AddOffset(IntStringMap::VT_VALUE, value); }
  IntStringMapBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  IntStringMapBuilder &operator=(const IntStringMapBuilder &);
  flatbuffers::Offset<IntStringMap> Finish() {
    auto o = flatbuffers::Offset<IntStringMap>(fbb_.EndTable(start_, 2));
    return o;
  }
};

inline flatbuffers::Offset<IntStringMap> CreateIntStringMap(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t id = 0,
   flatbuffers::Offset<flatbuffers::String> value = 0) {
  IntStringMapBuilder builder_(_fbb);
  builder_.add_value(value);
  builder_.add_id(id);
  return builder_.Finish();
}

struct ContainerMap FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_INT1 = 6,
    VT_TEXT1 = 8,
    VT_MAP1 = 10
  };
  int32_t id() const { return GetField<int32_t>(VT_ID, 0); }
  int64_t int1() const { return GetField<int64_t>(VT_INT1, 0); }
  const flatbuffers::String *text1() const { return GetPointer<const flatbuffers::String *>(VT_TEXT1); }
  const flatbuffers::Vector<flatbuffers::Offset<IntStringMap>> *map1() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<IntStringMap>> *>(VT_MAP1); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<int64_t>(verifier, VT_INT1) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_TEXT1) &&
           verifier.Verify(text1()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_MAP1) &&
           verifier.Verify(map1()) &&
           verifier.VerifyVectorOfTables(map1()) &&
           verifier.EndTable();
  }
};

struct ContainerMapBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) { fbb_.AddElement<int32_t>(ContainerMap::VT_ID, id, 0); }
  void add_int1(int64_t int1) { fbb_.AddElement<int64_t>(ContainerMap::VT_INT1, int1, 0); }
  void add_text1(flatbuffers::Offset<flatbuffers::String> text1) { fbb_.AddOffset(ContainerMap::VT_TEXT1, text1); }
  void add_map1(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<IntStringMap>>> map1) { fbb_.AddOffset(ContainerMap::VT_MAP1, map1); }
  ContainerMapBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  ContainerMapBuilder &operator=(const ContainerMapBuilder &);
  flatbuffers::Offset<ContainerMap> Finish() {
    auto o = flatbuffers::Offset<ContainerMap>(fbb_.EndTable(start_, 4));
    return o;
  }
};

inline flatbuffers::Offset<ContainerMap> CreateContainerMap(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t id = 0,
   int64_t int1 = 0,
   flatbuffers::Offset<flatbuffers::String> text1 = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<IntStringMap>>> map1 = 0) {
  ContainerMapBuilder builder_(_fbb);
  builder_.add_int1(int1);
  builder_.add_map1(map1);
  builder_.add_text1(text1);
  builder_.add_id(id);
  return builder_.Finish();
}

inline const flatbuf_bench::Root *GetRoot(const void *buf) { return flatbuffers::GetRoot<flatbuf_bench::Root>(buf); }

inline bool VerifyRootBuffer(flatbuffers::Verifier &verifier) { return verifier.VerifyBuffer<flatbuf_bench::Root>(); }

inline const char *RootIdentifier() { return "benc"; }

inline bool RootBufferHasIdentifier(const void *buf) { return flatbuffers::BufferHasIdentifier(buf, RootIdentifier()); }

inline const char *RootExtension() { return "fb_bench"; }

inline void FinishRootBuffer(flatbuffers::FlatBufferBuilder &fbb, flatbuffers::Offset<flatbuf_bench::Root> root) { fbb.Finish(root, RootIdentifier()); }

}  // namespace flatbuf_bench

#endif  // FLATBUFFERS_GENERATED_SERIALIZE_FLATBUF_BENCH_H_