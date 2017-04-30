
//------------------------------------------------------------------------------
// This code was generated by a tool.
//
//   Tool : Bond Compiler 0.4.1.0
//   File : serialize_types.h
//
// Changes to this file may cause incorrect behavior and will be lost when
// the code is regenerated.
// <auto-generated />
//------------------------------------------------------------------------------

#pragma once

#include <bond/core/bond_version.h>

#if BOND_VERSION < 0x302
#error This file was generated by a newer version of Bond compiler
#error and is incompatible with your version Bond library.
#endif

#if BOND_MIN_CODEGEN_VERSION > 0x0410
#error This file was generated by an older version of Bond compiler
#error and is incompatible with your version Bond library.
#endif

#include <bond/core/config.h>
#include <bond/core/containers.h>



namespace uti
{
namespace serialize
{
    
    namespace _bond_enumerators
    {
    namespace Enum1
    {
        enum Enum1
        {
            ONE,
            TWO,
            THREE,
            FOUR
        };
        
        extern const std::map<enum Enum1, std::string> _value_to_name_Enum1;
        extern const std::map<std::string, enum Enum1> _name_to_value_Enum1;

        inline
        const char* GetTypeName(enum Enum1)
        {
            return "Enum1";
        }

        inline
        const char* GetTypeName(enum Enum1, const bond::qualified_name_tag&)
        {
            return "uti.serialize.Enum1";
        }

        inline
        const std::map<enum Enum1, std::string>& GetValueToNameMap(enum Enum1)
        {
            return _value_to_name_Enum1;
        }

        inline
        const std::map<std::string, enum Enum1>& GetNameToValueMap(enum Enum1)
        {
            return _name_to_value_Enum1;
        }

        const std::string& ToString(enum Enum1 value);

        void FromString(const std::string& name, enum Enum1& value);

        inline
        bool ToEnum(enum Enum1& value, const std::string& name)
        {
            std::map<std::string, enum Enum1>::const_iterator it =
                _name_to_value_Enum1.find(name);

            if (_name_to_value_Enum1.end() == it)
                return false;

            value = it->second;

            return true;
        }
    } // namespace Enum1
    } // namespace _bond_enumerators

    using namespace _bond_enumerators::Enum1;
    

    
    struct ints
    {
        int64_t id;
        int64_t int1;
        int64_t int2;
        int32_t int3;
        int32_t int4;
        
        ints()
          : id(),
            int1(),
            int2(),
            int3(),
            int4()
        {
        }

        
#ifndef BOND_NO_CXX11_DEFAULTED_FUNCTIONS
        // Compiler generated copy ctor OK
        ints(const ints& other) = default;
#endif
        
#ifndef BOND_NO_CXX11_RVALUE_REFERENCES
        ints(ints&& other)
          : id(std::move(other.id)),
            int1(std::move(other.int1)),
            int2(std::move(other.int2)),
            int3(std::move(other.int3)),
            int4(std::move(other.int4))
        {
        }
#endif
        
        
#ifndef BOND_NO_CXX11_DEFAULTED_FUNCTIONS
        // Compiler generated operator= OK
        ints& operator=(const ints& other) = default;
#endif

        bool operator==(const ints& other) const
        {
            return true
                && (id == other.id)
                && (int1 == other.int1)
                && (int2 == other.int2)
                && (int3 == other.int3)
                && (int4 == other.int4);
        }

        bool operator!=(const ints& other) const
        {
            return !(*this == other);
        }

        void swap(ints& other)
        {
            using std::swap;
            swap(id, other.id);
            swap(int1, other.int1);
            swap(int2, other.int2);
            swap(int3, other.int3);
            swap(int4, other.int4);
        }

        struct Schema;

    protected:
        void InitMetadata(const char*, const char*)
        {
        }
    };

    inline void swap(ints& left, ints& right)
    {
        left.swap(right);
    }

    
    struct mixed
    {
        int32_t id;
        int32_t int1;
        int64_t int2;
        uint32_t uint1;
        double float1;
        std::string text1;
        ::uti::serialize::Enum1 enum1;
        
        mixed()
          : id(),
            int1(),
            int2(),
            uint1(),
            float1(),
            enum1(::uti::serialize::_bond_enumerators::Enum1::ONE)
        {
        }

        
#ifndef BOND_NO_CXX11_DEFAULTED_FUNCTIONS
        // Compiler generated copy ctor OK
        mixed(const mixed& other) = default;
#endif
        
#ifndef BOND_NO_CXX11_RVALUE_REFERENCES
        mixed(mixed&& other)
          : id(std::move(other.id)),
            int1(std::move(other.int1)),
            int2(std::move(other.int2)),
            uint1(std::move(other.uint1)),
            float1(std::move(other.float1)),
            text1(std::move(other.text1)),
            enum1(std::move(other.enum1))
        {
        }
#endif
        
        
#ifndef BOND_NO_CXX11_DEFAULTED_FUNCTIONS
        // Compiler generated operator= OK
        mixed& operator=(const mixed& other) = default;
#endif

        bool operator==(const mixed& other) const
        {
            return true
                && (id == other.id)
                && (int1 == other.int1)
                && (int2 == other.int2)
                && (uint1 == other.uint1)
                && (float1 == other.float1)
                && (text1 == other.text1)
                && (enum1 == other.enum1);
        }

        bool operator!=(const mixed& other) const
        {
            return !(*this == other);
        }

        void swap(mixed& other)
        {
            using std::swap;
            swap(id, other.id);
            swap(int1, other.int1);
            swap(int2, other.int2);
            swap(uint1, other.uint1);
            swap(float1, other.float1);
            swap(text1, other.text1);
            swap(enum1, other.enum1);
        }

        struct Schema;

    protected:
        void InitMetadata(const char*, const char*)
        {
        }
    };

    inline void swap(mixed& left, mixed& right)
    {
        left.swap(right);
    }

    
    struct complex
    {
        int64_t id;
        std::string text1;
        std::string text2;
        std::string text3;
        int32_t int1;
        int32_t int2;
        int32_t int3;
        std::string text4;
        ::uti::serialize::Enum1 enum1;
        double float1;
        int64_t int4;
        uint32_t uint1;
        uint64_t uint2;
        int64_t int5;
        int32_t int6;
        double float2;
        std::string text5;
        
        complex()
          : id(),
            int1(),
            int2(),
            int3(),
            enum1(::uti::serialize::_bond_enumerators::Enum1::ONE),
            float1(),
            int4(),
            uint1(),
            uint2(),
            int5(),
            int6(),
            float2()
        {
        }

        
#ifndef BOND_NO_CXX11_DEFAULTED_FUNCTIONS
        // Compiler generated copy ctor OK
        complex(const complex& other) = default;
#endif
        
#ifndef BOND_NO_CXX11_RVALUE_REFERENCES
        complex(complex&& other)
          : id(std::move(other.id)),
            text1(std::move(other.text1)),
            text2(std::move(other.text2)),
            text3(std::move(other.text3)),
            int1(std::move(other.int1)),
            int2(std::move(other.int2)),
            int3(std::move(other.int3)),
            text4(std::move(other.text4)),
            enum1(std::move(other.enum1)),
            float1(std::move(other.float1)),
            int4(std::move(other.int4)),
            uint1(std::move(other.uint1)),
            uint2(std::move(other.uint2)),
            int5(std::move(other.int5)),
            int6(std::move(other.int6)),
            float2(std::move(other.float2)),
            text5(std::move(other.text5))
        {
        }
#endif
        
        
#ifndef BOND_NO_CXX11_DEFAULTED_FUNCTIONS
        // Compiler generated operator= OK
        complex& operator=(const complex& other) = default;
#endif

        bool operator==(const complex& other) const
        {
            return true
                && (id == other.id)
                && (text1 == other.text1)
                && (text2 == other.text2)
                && (text3 == other.text3)
                && (int1 == other.int1)
                && (int2 == other.int2)
                && (int3 == other.int3)
                && (text4 == other.text4)
                && (enum1 == other.enum1)
                && (float1 == other.float1)
                && (int4 == other.int4)
                && (uint1 == other.uint1)
                && (uint2 == other.uint2)
                && (int5 == other.int5)
                && (int6 == other.int6)
                && (float2 == other.float2)
                && (text5 == other.text5);
        }

        bool operator!=(const complex& other) const
        {
            return !(*this == other);
        }

        void swap(complex& other)
        {
            using std::swap;
            swap(id, other.id);
            swap(text1, other.text1);
            swap(text2, other.text2);
            swap(text3, other.text3);
            swap(int1, other.int1);
            swap(int2, other.int2);
            swap(int3, other.int3);
            swap(text4, other.text4);
            swap(enum1, other.enum1);
            swap(float1, other.float1);
            swap(int4, other.int4);
            swap(uint1, other.uint1);
            swap(uint2, other.uint2);
            swap(int5, other.int5);
            swap(int6, other.int6);
            swap(float2, other.float2);
            swap(text5, other.text5);
        }

        struct Schema;

    protected:
        void InitMetadata(const char*, const char*)
        {
        }
    };

    inline void swap(complex& left, complex& right)
    {
        left.swap(right);
    }

    
    struct container_vec
    {
        int32_t id;
        int64_t int1;
        std::string text1;
        std::vector<std::string> stringvec;
        std::vector<int64_t> intvec;
        
        container_vec()
          : id(),
            int1()
        {
        }

        
#ifndef BOND_NO_CXX11_DEFAULTED_FUNCTIONS
        // Compiler generated copy ctor OK
        container_vec(const container_vec& other) = default;
#endif
        
#ifndef BOND_NO_CXX11_RVALUE_REFERENCES
        container_vec(container_vec&& other)
          : id(std::move(other.id)),
            int1(std::move(other.int1)),
            text1(std::move(other.text1)),
            stringvec(std::move(other.stringvec)),
            intvec(std::move(other.intvec))
        {
        }
#endif
        
        
#ifndef BOND_NO_CXX11_DEFAULTED_FUNCTIONS
        // Compiler generated operator= OK
        container_vec& operator=(const container_vec& other) = default;
#endif

        bool operator==(const container_vec& other) const
        {
            return true
                && (id == other.id)
                && (int1 == other.int1)
                && (text1 == other.text1)
                && (stringvec == other.stringvec)
                && (intvec == other.intvec);
        }

        bool operator!=(const container_vec& other) const
        {
            return !(*this == other);
        }

        void swap(container_vec& other)
        {
            using std::swap;
            swap(id, other.id);
            swap(int1, other.int1);
            swap(text1, other.text1);
            swap(stringvec, other.stringvec);
            swap(intvec, other.intvec);
        }

        struct Schema;

    protected:
        void InitMetadata(const char*, const char*)
        {
        }
    };

    inline void swap(container_vec& left, container_vec& right)
    {
        left.swap(right);
    }

    
    struct container_map
    {
        int32_t id;
        int64_t int1;
        std::string text1;
        std::map<int32_t, std::string> map1;
        
        container_map()
          : id(),
            int1()
        {
        }

        
#ifndef BOND_NO_CXX11_DEFAULTED_FUNCTIONS
        // Compiler generated copy ctor OK
        container_map(const container_map& other) = default;
#endif
        
#ifndef BOND_NO_CXX11_RVALUE_REFERENCES
        container_map(container_map&& other)
          : id(std::move(other.id)),
            int1(std::move(other.int1)),
            text1(std::move(other.text1)),
            map1(std::move(other.map1))
        {
        }
#endif
        
        
#ifndef BOND_NO_CXX11_DEFAULTED_FUNCTIONS
        // Compiler generated operator= OK
        container_map& operator=(const container_map& other) = default;
#endif

        bool operator==(const container_map& other) const
        {
            return true
                && (id == other.id)
                && (int1 == other.int1)
                && (text1 == other.text1)
                && (map1 == other.map1);
        }

        bool operator!=(const container_map& other) const
        {
            return !(*this == other);
        }

        void swap(container_map& other)
        {
            using std::swap;
            swap(id, other.id);
            swap(int1, other.int1);
            swap(text1, other.text1);
            swap(map1, other.map1);
        }

        struct Schema;

    protected:
        void InitMetadata(const char*, const char*)
        {
        }
    };

    inline void swap(container_map& left, container_map& right)
    {
        left.swap(right);
    }
} // namespace serialize
} // namespace uti

