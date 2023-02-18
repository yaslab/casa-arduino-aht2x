CXXFLAGS=-std=c++11 -O2 -Wall -Wextra -Wsign-conversion -I../src

PROGRAM=aht2x

SRCS=../src/AHT2x.cpp raspberrypi.cpp main.cpp
OBJS=$(SRCS:.cpp=.o)

all: $(PROGRAM)

clean:
	$(RM) $(PROGRAM) $(OBJS)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
