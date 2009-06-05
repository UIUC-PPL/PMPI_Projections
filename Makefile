
#MPI_DIR = $(HOME)/local/mpich2/
#MPICH_INC = -I$(MPI_DIR)/include
MPICH_LIB =  

# USE_STACK = -D__USE_STACK_TRACE_TO_IDENTIFY_TASKS__

MPICC = mpicc 
MPICXX = mpicxx 

CC = gcc -g
CXX = g++ -g

TARGETS = testprogram stack_test
objs = testprogram.o PMPI_ProjectionsLogging.o

all : $(TARGETS)

source_location.o : source_location.c source_location.h
	$(CC) -c source_location.c

PMPI_ProjectionsLogging.o : PMPI_ProjectionsLogging.C Makefile source_location.o
	$(MPICXX) -c PMPI_ProjectionsLogging.C -o PMPI_ProjectionsLogging.o $(MPICH_INC) 

testprogram.o : testprogram.c Makefile
	$(MPICXX) -c testprogram.c

testprogram : testprogram.o PMPI_ProjectionsLogging.o Makefile source_location.o
	$(MPICXX) PMPI_ProjectionsLogging.o testprogram.o  source_location.o $(MPICH_LIB) -o testprogram 

stack_test : stack_test.C source_location.o
	$(CXX) stack_test.C -o stacktest source_location.o

test : testprogram
	mpirun -n 4 ./testprogram

clean : 
	rm -rf *.o $(TARGETS) *.log $(objs) *.sts *.projrc *~ stacktest.dSYM stacktest 
 
