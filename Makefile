UNAME = $(shell uname)
ARCH = $(shell uname -m)

CXX = clang++
# OPTFLAGS = -g3
OPTFLAGS = -Ofast -march=native
CXXFLAGS = -std=c++17 $(OPTFLAGS) $(shell pkg-config --cflags glm epoxy glfw3) -I. -Wall -Wextra
LDFLAGS = $(shell pkg-config --libs glm epoxy glfw3)
OBJECTS = $(wildcard *.cpp)
BINARY = rubik

$(shell mkdir -p build)

all:;$(MAKE) glsl;$(MAKE) $(BINARY) -j$(shell getconf _NPROCESSORS_ONLN)

glsl:
	glslangValidator shader.vert shader.geom shader.frag
	glslangValidator shader.vert shader_strip.geom shader.frag

$(BINARY) : $(notdir $(patsubst %.cpp, %.o, $(OBJECTS)))
	$(CXX) $(wildcard build/*.o) $(CXXFLAGS) $(LDFLAGS) -o $(BINARY)

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o "build/$@" "$<"

clean :
	rm -rvf build
	rm -vf $(BINARY)
