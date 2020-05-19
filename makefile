CC=gcc
CXX=g++
CXXFLAGS=-std=c++11 -lpthread -fmax-errors=3
LDFLAGS= -lpthread

SRCS=piscope.cpp pi_spi_adc.cpp pi_screen.cpp pi_input.cpp pi_timer.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: piscope

piscope: $(OBJS) pi_input.h
	$(CXX) $(LDFLAGS) -o piscope $(OBJS) $(LDLIBS)

clean:
	rm -f core *.o
