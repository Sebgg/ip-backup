#
# Makefile för bruteforcelösning
#

CCC	= g++ -std=c++11 -g -O3
LFLAGS	= -lSDL -lm

all: brute fast

brute: brute.o point.o
	$(CCC) -o brute brute.o point.o $(LFLAGS)

fast: fast.o point.o
	$(CCC) -o fast fast.o point.o $(LFLAGS)

brute.o: brute.cpp point.h
	$(CCC) -o brute.o -c brute.cpp

fast.o: fast.cpp point.h
	$(CCC) -o fast.o -c fast.cpp

point.o: point.cpp point.h
	$(CCC) -o point.o -c point.cpp

clean:
	@ \rm -rf *.o

zap: clean
	@ \rm -f fast brute *~
