#
#
# Ktistakis Rafail
# CFSuID: crk12
# email: crk12@student.le.ac.uk
#
#
# ********************************
# *       countBefore		 *
# *        makefile              *
# ********************************
#
#
#

CC=g++ -std=c++11

FLAGS=-O3 #-DNDEBUG
FLAGS_SDSL_I=-I ~/include
FLAGS_SDSL_L=-L ~/lib
OFLAGS=-c
LIBS=-lsdsl -ldivsufsort -ldivsufsort64

EXEFLAGS=-o program.exe

	REM=rm -f
	XF=.exe
	IN=.o

clean:
	$(REM) *$(XF)
	$(REM) *$(IN)
	
countBefore.o: suffixArray.cpp suffixArray.h
	$(CC) $(OFLAGS) $(FLAGS) $(FLAGS_SDSL_I) suffixArray.cpp
	
all: main.cpp countBefore.o
	$(CC) $(FLAGS) $(FLAGS_SDSL_I) $(FLAGS_SDSL_L) suffixArray.o main.cpp $(EXEFLAGS) $(LIBS)
