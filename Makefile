UNAME = $(shell uname)
ARCH = $(shell uname -m)

CXXFLAGS = -std=c++1y -g2
LDFLAGS = $(shell ./link_graphics)
OBJECTS = $(wildcard *.cpp)
BINARY = rubik

$(shell mkdir -p build)

all:;$(MAKE) glsl;$(MAKE) $(BINARY) -j$(shell getconf _NPROCESSORS_ONLN)

glsl:
	glslangValidator $(wildcard *.vert)
	glslangValidator $(wildcard *.frag)

$(BINARY) : $(notdir $(patsubst %.cpp, %.o, $(OBJECTS)))
	$(CXX) $(wildcard build/*.o) $(CXXFLAGS) $(LDFLAGS) -o $(BINARY)

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o "build/$@" "$<"

clean :
	rm -rvf build
	rm -vf $(BINARY)
