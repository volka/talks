CFLAGS = -O2 -std=c++14
INCLUDE = -I.

BOND_PROG = bond_serialize
BOND_SOURCES = bond_serialize.cpp serialize_apply.cpp serialize_types.cpp
all: $(BOND_PROG)
	g++ $(CFLAGS) $(INCLUDE) $(BOND_SOURCES) -o $(BOND_PROG)

