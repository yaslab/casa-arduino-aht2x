CXXFLAGS=-std=c++11 -O2 -Wall -Wextra -Wsign-conversion -I../../src

PROGRAM=aht2x

SRCS=$(wildcard ../../src/*.cpp) $(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(PROGRAM)

clean:
	$(RM) $(PROGRAM) $(OBJS)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
