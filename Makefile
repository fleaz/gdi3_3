# For debugging purposes, set -O0 and -g.
# For production use, set -O3.
CXXFLAGS = -O3 -g -Wall -pedantic
CXX = g++
COMPILE.cc = ${CXX} ${CXXFLAGS} ${CPPFLAGS} -c

SOURCES := $(wildcard [^_]*.cc)
OBJECTS := ${SOURCES:.cc=.o}
BINARY = bvh

all: $(OBJECTS)
	$(CXX) -o $(BINARY) $(OBJECTS)

%.o: %.cc
	${COMPILE.cc} -o $@ $<

clean:
	$(RM) $(BINARY) $(OBJECTS)
