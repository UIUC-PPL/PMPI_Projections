# (c) 2009 Isaac Dooley
CXX = g++ -g

TARGETS = libpmpiprojections.a
LIBOBJS =  PMPI_ProjectionsLogging.o

all : $(TARGETS)

libpmpiprojections.a : $(LIBOBJS)
	ar rvs libpmpiprojections.a $(LIBOBJS)

PMPI_ProjectionsLogging.o : PMPI_ProjectionsLogging.C Makefile PMPI_ProjectionsLogging.h 
	$(CXX) -c PMPI_ProjectionsLogging.C -o PMPI_ProjectionsLogging.o 

# testprogram.o : testprogram.c Makefile
# 	$(MPICXX) -c testprogram.c -O0 -g

# testprogram :  Makefile libpmpiprojections.a testprogram.o
# 	$(MPICXX) testprogram.o  -L. -lpmpiprojections $(MPICH_LIB) -o testprogram 

# testprogram-no-PMPI : testprogram.o Makefile source_location.o
# 	$(MPICXX) testprogram.o  $(MPICH_LIB) -o testprogram-no-PMPI 

# test : testprogram
# 	mpirun -n 4 ./testprogram

# test-no-PMPI : testprogram-full
# 	mpirun -n 4 ./testprogram-no-PMPI

clean : 
	rm -rf *.o $(TARGETS) *.log $(OBJS) *.sts *.projrc *~  libpmpiprojections.a
