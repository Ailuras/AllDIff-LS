/*
 * cpProcedure.cpp
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */


#include <time.h>
#include <set>


void cpProcedure(int **grid, bool **fixed, int sqLen, int grLen, int nSq,
	int first, int last);
bool executionLoop(int **grid, bool **fixed, int sqLen, int grLen, int nSq,
		int first, int last);
void checkForOnlyOnePossibleValue(int **grid, bool **fixed, int sqLen, int grLen,
		int nSq, int first, int last, int x, int y);
std::set<int> getListWithPossibleValues(int **grid, bool **fixed, int sqLen,
		int first, int last, int x, int y);
