UNAME = $(shell uname)
ARCH = $(shell uname -m)

CXX ?= clang++
CXXFLAGS = -std=c++17 -g2 $(shell pkg-config --cflags glm epoxy glfw3) -I.
LDFLAGS = $(shell pkg-config --libs glm epoxy glfw3)
OBJECTS = $(wildcard *.cpp)
BINARY = rubik

$(shell mkdir -p build)

all:;$(MAKE) glsl;$(MAKE) $(BINARY) -j$(shell getconf _NPROCESSORS_ONLN)

glsl:
	glslangValidator shader.vert shader.geom shader.frag
	glslangValidator shader.vert shader_strip.geom shader.frag
	glslangValidator box.vert shader.geom shader.frag
	glslangValidator box.vert shader_strip.geom shader.frag

$(BINARY) : $(notdir $(patsubst %.cpp, %.o, $(OBJECTS)))
	$(CXX) $(wildcard build/*.o) $(CXXFLAGS) $(LDFLAGS) -o $(BINARY)

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o "build/$@" "$<"

clean :
	rm -rvf build
	rm -vf $(BINARY)
