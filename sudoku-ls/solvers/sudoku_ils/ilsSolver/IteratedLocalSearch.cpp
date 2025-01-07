/*
 * IteratedLocalSearch.cpp
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#include "IteratedLocalSearch.h"
#include "Util.h"
#include "TabuSearch.h"
#include <iostream>
#include <utility>
#include "../logger/Logger.h"

#include "Constants.h"


IteratedLocalSearch::IteratedLocalSearch() {

}

IteratedLocalSearch::~IteratedLocalSearch() {

}



/**
 * Executes an Iterated Local Search on a given Sudoku
 */
Sudoku IteratedLocalSearch::iteratedLocalSearch(Sudoku sudoku, int tabuListSize, long timeLimit, long mcProbability, int iterationLimit) {
	long starttime = Util::time();
	Sudoku bestSudoku = sudoku;

	int cost = sudoku.getCost();
	int bestCost = cost;
	int numberOfCells = sudoku.getNumrows()*sudoku.getNumrows();
	float resets = 1;


	while (cost > 0) {
		sudoku = TabuSearch::tabuSearch(sudoku, tabuListSize, timeLimit-(Util::time() - starttime), mcProbability, iterationLimit);
		cost = sudoku.getCost();

		if (cost > 0) {
			if (cost < bestCost) {
				bestSudoku = sudoku;
				bestCost = cost;
			}
		}
		else {
			bestSudoku = sudoku;
		}

		Logger::getInstance().logInfo("Checking time...");
		if (Util::time() - starttime > timeLimit) {
			Logger::getInstance().logInfo("Out of time!");
			break;
		}


		if (cost > 0) {
			sudoku = TabuSearch::tryCpApproach(sudoku, resets);
			if (resets > 0.01) {
				resets *= ALPHA;
			}
			cost = sudoku.getCost();

			if (cost < bestCost) {
				bestSudoku = sudoku;
			}
		}
	}

	return bestSudoku;
}


