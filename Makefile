
MPI_DIR = $(HOME)/local/mpich2/

MPICH_INC = -I$(MPI_DIR)/include
MPICH_LIB = -L$(MPI_DIR)/lib  -lpmpich -lmpich
MPICC = $(MPI_DIR)/bin/mpicc 
MPICXX = $(MPI_DIR)/bin/mpicxx 

CC = gcc -g
CXX = g++ -g

TARGETS = testprogram 
objs = testprogram.o PMPI_ProjectionsLogging.o

all : $(TARGETS)

PMPI_ProjectionsLogging.o : PMPI_ProjectionsLogging.C Makefile
	$(CXX) -c PMPI_ProjectionsLogging.C -o PMPI_ProjectionsLogging.o $(MPICH_INC) 

testprogram.o : testprogram.c Makefile
	$(MPICXX) -c testprogram.c

testprogram : testprogram.o PMPI_ProjectionsLogging.o Makefile
	$(CXX) PMPI_ProjectionsLogging.o testprogram.o $(MPICH_LIB) -o testprogram 
	
clean : 
	rm -f *.o $(TARGETS) *.log $(objs) *.sts *.projrc
 
