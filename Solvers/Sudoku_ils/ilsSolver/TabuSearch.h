/*
 * TabuSearch.h
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#ifndef TABUSEARCH_H_
#define TABUSEARCH_H_

#include "Sudoku.h"

/** Stores the coordinates of two different fields that were swapped or shall be swapped */
struct Swap {
	int x, y, swapx, swapy;
};

class TabuSearch {

/**
*  Represents a Neighbour solution.
*
*  Stores the underlying sudoku, its cost and the last swap that was done to generate this solution.
*/
struct Neighbour {
	Sudoku sudoku;
	int cost;
	int numSwaps;
	std::vector<Swap> swaps;
};

private:
	TabuSearch();
	virtual ~TabuSearch();

public:
	static Sudoku tabuSearch(Sudoku sudoku, int tabulistsize, long timeLimit, long mcProbability, int iterationLimit);

	static Sudoku generateInitialSolution(Sudoku sudoku);
	static int evaluateSolution(Sudoku sudoku);
	static std::vector<Neighbour> generateNeighbourhood(Sudoku sudoku, bool wholeNbHood=true);

	static Neighbour getNextSolutionFromNeighbourhood(Sudoku currentSolution, std::deque<std::vector<Swap> > tabulist, int bestcost, bool wholeNbHood=true);
	static Neighbour getNextSolutionByRandomWalk(Sudoku currentSolution);
	static Neighbour getNextSolutionByMCWithTabulist(Sudoku currentSolution,
			std::vector<std::vector<std::vector<std::vector<int> > > >& tabulist, int tabulistSize, int bestCost, int , int iteration);

	static Neighbour getNeighbour(Sudoku sudoku, int x, int y);
	static std::vector<Neighbour> getPossibleNeighbours(Sudoku sudoku, int x, int y);

	static std::vector<Swap> minConflictsWithTabuList(Sudoku sudoku, std::vector<std::pair<int,int> > xys, std::deque<std::vector<Swap> > tabulist, int bestcost, int currentCost);

	static Sudoku tryCpApproach(Sudoku, float resets);

	static void printTabuList(std::deque<std::vector<Swap> > tabulist);
	static bool isTabu(std::deque<std::vector<Swap> > tabulist, std::vector<Swap>);

	static std::pair<std::vector<Swap>, int> findBestSwap(Sudoku sudoku, std::vector<std::pair<int, int> > xys, std::deque<std::vector<Swap> > tabulist, int bestCost, int currentCost, std::vector<Swap> swaps, int swapLimit);


};

#endif /* TABUSEARCH_H_ */
