/*
 * btProcedure.h
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#include <time.h>
#include <string>

bool btProcedure(int **grid, bool **fixed, int sqLen, int grLen, int nSq,
	int first, int last, bool minDomainFirst, std::string heuristic,
	clock_t finishTime);
