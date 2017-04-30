@0xc95f38542f78fdf9;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("capnp_bench");

enum ENUMERATION {
   one @0;
   two @1;
   three @2;
   four @3;
}

struct Ints {
   id   @0 : Int64;
   int1 @1 : Int64;
   int2 @2 : Int64;
   int3 @3 : Int32;
   int4 @4 : Int32;
}

struct IntList {
   ints @0 : List(Ints);
}

struct Mixed {
   id    @0 : Int32;
   int1  @1 : Int32;
   int2  @2 : Int64;
   uint1 @3 : UInt32;
   float1 @4 : Float64;
   text1 @5 : Text;
   enum1 @6 : ENUMERATION;
}

struct MixedList {
   mixed @0 : List(Mixed);
}

struct ContainerVec {
   id    @0 : Int32;
   int1  @1 : Int64;
   text1 @2 : Text;
   stringvec @3 : List(Text);
   intvec @4 : List(Int64);
}

struct ContainerVecList {
   contvec @0 : List(ContainerVec);
}

struct Map(Key, Value) {
   entries @0 : List(Entry);
   struct Entry {
      key @0 :Key;
      value @1 :Value;
   }
}

struct IntKey {
   id @0 : Int32;
}

struct StringMap {
   byId @0 :Map(IntKey, Text);
}

struct ContainerMap {
   id    @0 : Int32;
   int1  @1 : Int64;
   text1 @2 : Text;
   map1  @3 : StringMap;
}

struct ContainerMapList {
   contmap @0 : List(ContainerMap);
}

struct Complex {
   id    @0 : Int64;
   text1 @1 : Text;
   text2 @2 : Text;
   text3 @3 : Text;
   int1  @4 : Int32;
   int2  @5 : Int32;
   int3  @6 : Int32;
   text4 @7 : Text;
   enum1 @8 : ENUMERATION;
   float1 @9 : Float64;
   int4  @10 : Int64;
   uint1 @11 : UInt32;
   uint2 @12 : UInt64;
   int5  @13 : Int64;
   int6  @14 : Int32;
   float2 @15 : Float64;
   text5 @16 : Text;
}

struct ComplexList {
   complex @0 : List(Complex);
}
