CFLAGS=-g -std=c++11 -O2 -march=native

all: tmp
tmp: tmp.cpp $(wildcard *.hpp)
	$(CXX) $(CFLAGS) $< -o $@
