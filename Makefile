# (c) 2009 Isaac Dooley

# ------------------------------------------------
# Enable one of the following files. 
# For MPI 1 routines: 
MPI_PROTOTYPES_FILE = mpi.h-v1-sanitized
# For MPI 2 routines: 
#MPI_PROTOTYPES_FILE = mpi.h-v2-sanitized
# ------------------------------------------------


# USE_STACK = -D__USE_STACK_TRACE_TO_IDENTIFY_TASKS__

MPICC = mpicc -g
MPICXX = mpicxx -g

CC = gcc -g
CXX = g++ -g

TARGETS = testprogram 
objs = testprogram.o PMPI_ProjectionsLogging.o

all : $(TARGETS)

source_location.o : source_location.c source_location.h
	$(CC) -c source_location.c

PMPI_ProjectionsLogging.o : PMPI_ProjectionsLogging.C Makefile source_location.o generated-eventids.h PMPI_ProjectionsLogging.h
	$(MPICXX) -c PMPI_ProjectionsLogging.C -o PMPI_ProjectionsLogging.o $(MPICH_INC) 

generated-definitions.o : generated-definitions.C  generated-eventids.h PMPI_ProjectionsLogging.h
	$(MPICXX) -c generated-definitions.C -o generated-definitions.o $(MPICH_INC) 

testprogram.o : testprogram.c Makefile
	$(MPICXX) -c testprogram.c -O0 -g

testprogram : testprogram.o PMPI_ProjectionsLogging.o Makefile source_location.o generated-definitions.o
	$(MPICXX) PMPI_ProjectionsLogging.o testprogram.o source_location.o generated-definitions.o $(MPICH_LIB) -o testprogram 

testprogram-no-PMPI : testprogram.o Makefile source_location.o
	$(MPICXX) testprogram.o source_location.o  $(MPICH_LIB) -o testprogram-no-PMPI 


stack_test : stack_test.C source_location.o
	$(CXX) stack_test.C -o stacktest source_location.o

test : testprogram
	mpirun -n 4 ./testprogram

test-no-PMPI : testprogram-full
	mpirun -n 4 ./testprogram-no-PMPI


generated-definitions.C generated-eventids.h : $(MPI_PROTOTYPES_FILE) generate_pmpi_wrappers.pl
	 perl generate_pmpi_wrappers.pl  $(MPI_PROTOTYPES_FILE)



clean : 
	rm -rf *.o $(TARGETS) *.log $(objs) *.sts *.projrc *~ stacktest.dSYM stacktest generated-definitions.C generated-stsEvents.txt generated-sts.C generated-eventids.h  
 
