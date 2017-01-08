UNAME = $(shell uname)
ARCH = $(shell uname -m)

CFLAGS = -std=c++1y $(shell cat ./_cflags)
OBJECTS = $(wildcard *.cpp)
BINARY = rubik

all : $(BINARY)
	$(CXX) $(CFLAGS) $(OBJECTS) -o "$(BINARY)"

clean :
	rm -vf $(BINARY)
