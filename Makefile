# (c) 2009 Isaac Dooley

#MPI_DIR = $(HOME)/local/mpich2/
#MPICH_INC = -I$(MPI_DIR)/include
#MPICH_LIB =  

# USE_STACK = -D__USE_STACK_TRACE_TO_IDENTIFY_TASKS__

MPICC = mpicc -g
MPICXX = mpicxx -g

CC = gcc -g
CXX = g++ -g

TARGETS = testprogram stack_test
objs = testprogram.o PMPI_ProjectionsLogging.o

all : $(TARGETS)

source_location.o : source_location.c source_location.h
	$(CC) -c source_location.c

PMPI_ProjectionsLogging.o : PMPI_ProjectionsLogging.C Makefile source_location.o generated-eventids.h PMPI_ProjectionsLogging.h
	$(MPICXX) -c PMPI_ProjectionsLogging.C -o PMPI_ProjectionsLogging.o $(MPICH_INC) 

generated-definitions.o : generated-definitions.C  generated-eventids.h PMPI_ProjectionsLogging.h
	$(MPICXX) -c generated-definitions.C -o generated-definitions.o $(MPICH_INC) 

testprogram.o : testprogram.c Makefile
	$(MPICXX) -c testprogram.c

testprogram : testprogram.o PMPI_ProjectionsLogging.o Makefile source_location.o generated-definitions.o
	$(MPICXX) PMPI_ProjectionsLogging.o testprogram.o source_location.o generated-definitions.o $(MPICH_LIB) -o testprogram 

stack_test : stack_test.C source_location.o
	$(CXX) stack_test.C -o stacktest source_location.o

test : testprogram
	mpirun -n 4 ./testprogram

test-full : testprogram-full
	mpirun -n 4 ./testprogram-full


generated-definitions.C generated-eventids.h : mpi.h-sanitized generate_pmpi_wrappers.pl
	 perl generate_pmpi_wrappers.pl mpi.h-sanitized 



clean : 
	rm -rf *.o $(TARGETS) *.log $(objs) *.sts *.projrc *~ stacktest.dSYM stacktest generated-definitions.C generated-stsEvents.txt generated-sts.C generated-eventids.h  
 
