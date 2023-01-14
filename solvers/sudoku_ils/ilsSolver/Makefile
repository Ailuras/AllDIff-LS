############################# Makefile ##########################
# Part of the Source Code for the Sudoku solver described in
# "A Hybrid Heuristic Approach for the Sudoku Problem"
#

all: ilsSolver

ilsSolver: Util.o IteratedLocalSearch.o TabuSearch.o btProcedure.o cpProcedure.o Sudoku.o Logger.o main.o
	g++ -g -o ilsSolver Util.o IteratedLocalSearch.o TabuSearch.o btProcedure.o cpProcedure.o Sudoku.o Logger.o main.o

main.o: main.cpp Constants.h
	g++ -g -c main.cpp

Util.o: Util.cpp Util.h
	g++ -g -c Util.cpp

IteratedLocalSearch.o: IteratedLocalSearch.cpp IteratedLocalSearch.h Constants.h
	g++ -g -c IteratedLocalSearch.cpp
	
TabuSearch.o: TabuSearch.cpp TabuSearch.h Constants.h
	g++ -g -c TabuSearch.cpp
	
Sudoku.o: Sudoku.cpp Sudoku.h Constants.h
	g++ -g -c Sudoku.cpp
	
btProcedure.o: ../cpProcedure/btProcedure.cpp ../cpProcedure/btProcedure.h
	g++ -g -c ../cpProcedure/btProcedure.cpp
	
cpProcedure.o: ../cpProcedure/cpProcedure.cpp ../cpProcedure/cpProcedure.h
	g++ -g -c ../cpProcedure/cpProcedure.cpp
	
Logger.o: ../logger/Logger.cpp ../logger/Logger.h
	g++ -g -c ../logger/Logger.cpp
	
clean:
	rm -rf *.o ilsSolver
