CC=gcc
CXX=g++
CXXFLAGS=-std=c++17 -lpthread -lbcm2835 -fmax-errors=3 -flto -Ofast
DUMP=
LDFLAGS= -lpthread -lbcm2835
LDLIBS= -lbcm2835

SRCS=piscope.cpp pi_spi_adc.cpp pi_screen.cpp pi_input.cpp pi_timer.cpp pi_controller.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: piscope

piscope: $(OBJS) pi_input.h
	$(CXX) $(LDFLAGS) -o piscope $(OBJS) $(LDLIBS)

clean:
	rm -f core *.o
