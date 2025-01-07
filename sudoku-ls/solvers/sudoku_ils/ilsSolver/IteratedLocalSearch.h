/*
 * IteratedLocalSearch.h
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#ifndef ITERATEDLOCALSEARCH_H_
#define ITERATEDLOCALSEARCH_H_

#include "Sudoku.h"

class IteratedLocalSearch {
private:
	IteratedLocalSearch();
	virtual ~IteratedLocalSearch();

public:
	static Sudoku iteratedLocalSearch(Sudoku sudoku, int tabufactor, long timeLimit, long tabuProbability, int iterationLimit);
};

#endif /* ITERATEDLOCALSEARCH_H_ */
