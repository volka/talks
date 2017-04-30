#include "serialize_reflection.h"
#include <bond/python/struct.h>

BOOST_PYTHON_MODULE(uti::serialize)
{
	bond::python::struct_<uti::serialize::ints>.def();
}

