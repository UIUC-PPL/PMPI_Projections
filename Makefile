# (c) 2009 Isaac Dooley
CC = gcc -g
CXX = g++ -g

TARGETS = libpmpiprojections.a testprogram
LIBOBJS =  PMPI_ProjectionsLogging.o

all : $(TARGETS)

libpmpiprojections.a : $(LIBOBJS)
	ar rvs libpmpiprojections.a $(LIBOBJS)

PMPI_ProjectionsLogging.o : PMPI_ProjectionsLogging.C Makefile PMPI_ProjectionsLogging.h 
	$(CXX) -c PMPI_ProjectionsLogging.C -o PMPI_ProjectionsLogging.o 

testprogram.o : testprogram.c Makefile
	$(CC) -c testprogram.c -O0 -g

testprogram :  Makefile libpmpiprojections.a testprogram.o
	$(CC) testprogram.o  -L. -lpmpiprojections -lstdc++ -o testprogram

test : testprogram
	./testprogram

clean : 
	rm -rf *.o $(TARGETS) *.log $(OBJS) *.sts *.projrc *~  libpmpiprojections.a
