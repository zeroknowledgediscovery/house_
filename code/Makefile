CC = g++ -std=c++11

CFLAGS = -O3 -Wall -Wextra -Wunused -fopenmp -Wl,--as-needed
CFLAGS =  -static -O3 -Wall -Wextra -Wunused -fopenmp -Wl,--as-needed

ZBASE=./#
LIBSO=  -lgomp  -lm 
LIBS=   -lboost_system -lboost_thread -lboost_program_options -lboost_timer  -lboost_chrono -Bdynamic -lgsl -lgslcblas

LIBSO=  -static-libstdc++ -static-libgcc -lgomp  -lm 
LIBS=   -lboost_system -lboost_thread -lboost_filesystem -lboost_program_options -lboost_timer  -lboost_chrono -Bdynamic -lgsl -lgslcblas
LIBPATH= $(ZBASE)/lib

DEPS = 
INCLUDES = -I$(ZBASE)

OBJ =  procdb extract_age_group

all:	 $(OBJ)  clear mvbin


# compile libraries --------------------------------
semantic.o: semantic.cc $(DEPS)
	$(CC) -c -o $@ $< -fopenmp $(CFLAGS) $(INCLUDES); ar rcs libsemcrct.a semantic.o; rm semantic.o; mv libsemcrct.a ./lib

config.o: config.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS); ar rcs libconfigfile.a config.o; rm config.o; mv libconfigfile.a ./lib

procx.o: procx.cc 
	$(CC) -c -o $@ $< $(CFLAGS); ar rcs libdictx_.a procx.o; rm procx.o; mv libdictx_.a ./lib

# compile binaries --------------------------------

%.o :	%.cc
	$(CC) $(INCLUDES)  -c -o $@ $< $(CFLAGS)

extract_age_group:	extract_age_group.o
	$(CC)  $(CFLAGS) -o $@ $^   -L$(LIBPATH)  -ldictx_  $(LIBSO)  $(LIBS)  

procscript:	procscript.o
	$(CC)  $(CFLAGS) -o $@ $^   -L$(LIBPATH)  -ldictx_  $(LIBSO)  $(LIBS)  

procdb:	procdb.o
	$(CC)  $(CFLAGS) -o $@ $^   -L$(LIBPATH)  -ldictx_  $(LIBSO)  $(LIBS)  

corr: corr.o
	$(CC)  $(CFLAGS) -o $@ $^   -L$(LIBPATH) $(LIBSO) $(LIBS) 
#-Bdynamic -lgsl 


cnfbd: cnfbd.o
	$(CC)  $(CFLAGS) -o $@ $^  -L$(LIBPATH)  $(LIBSO) $(LIBS) 
#-Bdynamic -lgsl 


# utility commands --------------------------------

clear:	
	rm -rf *.o
clean:	
	rm -rf *.o *~ $(OBJ) *.tgz

mvbin:
	rm -rf ./bin >&/dev/null; mkdir bin; mv $(OBJ) ./bin
src:
	tar -czvf src.tgz *.cc *.h Makefile Doxyfile
ref:
	doxygen Doxyfile; cd doc/latex; make; make

 # EOF --------------------------------
