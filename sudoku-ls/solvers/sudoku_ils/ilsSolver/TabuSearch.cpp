/*
 * TabuSearch.cpp
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#include "TabuSearch.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <map>
#include <utility>
#include "Util.h"
#include "../logger/Logger.h"
#include "Constants.h"

bool operator==(const Swap &a, const Swap &b)
{
	return (a.x == b.x && a.y == b.y) && (a.swapx == b.swapx && a.swapy == b.swapy);
}

TabuSearch::TabuSearch() {
}

TabuSearch::~TabuSearch() {
}

/**
 * Generates an initial Solution by filling all the empty cells of a given Sudoku.
 * This is done randomly in such a way that no conflicts exist inside any square.
 *
 * @return A Sudoku containing the generated solution.
 */
Sudoku TabuSearch::generateInitialSolution(Sudoku sudoku) {
	int order = sudoku.getOrder();
	int numrows = sudoku.getNumrows();
	std::vector< std::vector<int> > missingvalues;

	std::set<int> referenceSet;
	for (int j=0; j<numrows+1; j++) {
		referenceSet.insert(j);
	}

	for (int i=0; i<numrows; i++) {
		std::set<int> sudokuset;
		std::vector<int> setdiff;

		sudokuset = sudoku.getSquare(i);
		std::set_difference(referenceSet.begin(), referenceSet.end(), sudokuset.begin(), sudokuset.end(), std::inserter(setdiff, setdiff.end()));
		random_shuffle(setdiff.begin(), setdiff.end());
		missingvalues.push_back(setdiff);
	}

	for (int i=0; i<numrows; i++) {
		int rowstart = (i/order)*order;
		int colstart = (i%order)*order;

		for (int j=rowstart; j<rowstart+order; j++) {
			for (int k=colstart; k<colstart+order; k++) {
				if (sudoku.getValue(j, k) == 0) {
					sudoku.setValue(j, k, missingvalues[i].back());
					missingvalues[i].pop_back();
				}
			}
		}
	}

	for (int i=0; i<numrows; i++) {
		sudoku.setColCost(i);
		sudoku.setRowCost(i);
	}

	return sudoku;
}

int TabuSearch::evaluateSolution(Sudoku sudoku) {
	return sudoku.getCost();
}

/**
 * Generates the neighbourhood of given solution, by finding neighbours for all cells that can be swapped.
 *
 * @param wholeNbHood If true, all possible swaps are determined for each possible cell.
 *                    Otherwise only one swap per cell is determined randomly.
 *                    wholeNbHood is true by default.
 */
std::vector<TabuSearch::Neighbour> TabuSearch::generateNeighbourhood(Sudoku sudoku, bool wholeNbHood) {
	std::vector<TabuSearch::Neighbour> neighbours, newNeighbours;
	int numrows = sudoku.getNumrows();

	for (int i=0; i<numrows; i++) {
		for (int j=0; j<numrows; j++) {
			if (!sudoku.isFixed(i,j)) {
				if (wholeNbHood) {
					newNeighbours = TabuSearch::getPossibleNeighbours(sudoku, i, j);
					neighbours.insert(neighbours.end(), newNeighbours.begin(), newNeighbours.end());
				}
				else {
					neighbours.push_back(TabuSearch::getNeighbour(sudoku, i, j));
				}
			}
		}
	}

	return neighbours;
}

/**
 * Finds a solution for a given sudoku by applying TabuSearch and MinConflictsHeuristic algorithms.
 *
 * @param tabuListSize Every applied swap is tabu for a certain number of following iterations.
 *                     The parameter tabuListSize sets this number.
 * @param timeLimit The will end if either an optimal solution is found, or if the given timeLimit (in msecs) has passed.
 * @param mcProbability A value between 0 and 100. It determines the probability with that MinConflicts is used in each iteration.
 *                        With probability 100-tabuProbability RandomWalk is used.
 * @param iterationLimit The iterationlimit set by IteratedLocalSearch. The search is cancelled after the number
 * 						  of given iterations even if no solution has been found.
 */
Sudoku TabuSearch::tabuSearch(Sudoku sudoku, int tabuListSize, long timeLimit, long mcProbability, int iterationLimit) {
	std::vector<std::vector<std::vector<std::vector<int> > > >
	tabulist(sudoku.getNumrows(), std::vector<std::vector<std::vector<int> > >(sudoku.getNumrows(), std::vector<std::vector<int> >(sudoku.getNumrows(), std::vector<int>(sudoku.getNumrows(), -1))));
	bool end = false;
	int bestcost;
	int currentcost;

	long starttime = Util::time();
	int iterations = 0;
	int initialIterationLimit = iterationLimit;
	int numberOfCells = sudoku.getNumrows()*sudoku.getNumrows();

	// generate initial solution
	Sudoku currentSolution = TabuSearch::generateInitialSolution(sudoku);
	int numUnfixed = currentSolution.getNumberOfUnfixedCells();
	Sudoku bestSolution = currentSolution;



	// Evaluate initial solution
	bestcost = currentSolution.getCost();
	currentcost = currentSolution.getCost();

	while (!end) {

		Neighbour currentNeighbour;

		if (Util::randomInt(1, 100) <= mcProbability) {
			currentNeighbour = TabuSearch::getNextSolutionByMCWithTabulist(currentSolution, tabulist, tabuListSize, bestcost, currentcost, iterations);
		}
		else {
			currentNeighbour = TabuSearch::getNextSolutionByRandomWalk(currentSolution);
		}

		if (currentcost >= currentNeighbour.cost || Util::randomInt(0, 100) < ACCEPTANCE_PROB) {

		currentSolution = currentNeighbour.sudoku;
		currentcost = currentNeighbour.cost;

		if (currentcost < bestcost) {
			bestcost = currentcost;
			bestSolution = currentSolution;
			iterationLimit = iterations + initialIterationLimit;
		}
		}

		std::stringstream ss2;
		ss2 << iterations << " " << currentcost;

		Logger::getInstance().logInfo(ss2.str());

		// update tabu
		Swap swap = currentNeighbour.swaps[0];
		tabulist[swap.x][swap.y][swap.swapx][swap.swapy] = iterations;


		iterations++;

		// terminate condition
		if (iterations > iterationLimit) {
			end = true;
		}
		else if (currentcost == 0) {
			end = true;
		}
		else if (Util::time()-starttime > timeLimit) {
			end = true;
		}
	}

	return bestSolution;
}

/**
 * Swaps the given field with another randomly chosen field of the same square which is not fixed.
 */
TabuSearch::Neighbour TabuSearch::getNeighbour(Sudoku sudoku, int x, int y) {

	int order = sudoku.getOrder();
	int swapx_offset = x - x % order;
	int swapy_offset = y - y % order;
	int swapx, swapy;
	bool swapPossible = false;

	std::vector<std::pair<int, int> > possibleSlots;

	// check if swap with this cell is possible
	for (int i=0; i<order; i++) {
		for (int j=0; j<order; j++) {
			swapx = swapx_offset + i;
			swapy = swapy_offset + j;

			if (!sudoku.isFixed(swapx, swapy) && (x != swapx || y != swapy)) {
				possibleSlots.push_back(std::pair<int, int>(swapx, swapy));
			}
		}
	}

	Neighbour neighbour;
	Swap swap;


	if (possibleSlots.size() == 0) {
		swap.x = x;
		swap.y = y;
		neighbour.swaps.push_back(swap);
		neighbour.sudoku = sudoku;
		neighbour.cost = sudoku.getCost();
		return neighbour;
	}

	int rand = Util::randomInt(0, possibleSlots.size()-1);

	std::pair<int, int> slot = possibleSlots[rand];
	swapx = slot.first;
	swapy = slot.second;





	int value1 = sudoku.getValue(x, y);
	int value2 = sudoku.getValue(swapx, swapy);

	sudoku.setValue(x, y, value2);
	sudoku.setValue(swapx, swapy, value1);

	neighbour.sudoku = sudoku;
	swap.x = x;
	swap.y = y;
	swap.swapx = swapx;
	swap.swapy = swapy;
	neighbour.swaps.push_back(swap);
	neighbour.cost = sudoku.getCost();

	return neighbour;
}

/**
 * Achieves a Neighbour for a given currentSolution.
 *
 * First all possible Neighbours are generated, then the Neighbour with the best cost is taken.
 * If multiple Neighbours with the same cost exist, one of them is chosen randomly.
 *
 * @param tabulist The tabulist determines which swaps are tabu. Neighbours generated by applying a tabu swap
 *                 will only be taken if they beat the best cost so far.
 * @param bestcost The best cost in this search so far.
 * @param wholeNbHood If true, every possible swap for every possible cell will be considered.
 *                    If false, only one randomly chosen swap will be considered for each cell.
 */
TabuSearch::Neighbour TabuSearch::getNextSolutionFromNeighbourhood(Sudoku currentSolution,
		std::deque<std::vector<Swap> > tabulist, int bestcost, bool wholeNbHood) {

	// Generate Neighbourhood
	std::vector<TabuSearch::Neighbour> neighbours = generateNeighbourhood(currentSolution, wholeNbHood);
	std::multimap<int, TabuSearch::Neighbour> neighbours_wcost;

	random_shuffle(neighbours.begin(), neighbours.end());
	for (std::vector<TabuSearch::Neighbour>::iterator iter = neighbours.begin();
			iter != neighbours.end(); ++iter) {

		neighbours_wcost.insert(std::pair<int,TabuSearch::Neighbour>(iter->cost, *iter));
	}

	bool nextSolutionFound = false;
	TabuSearch::Neighbour currentNeighbour;

	while (!nextSolutionFound) {
		currentNeighbour = neighbours_wcost.begin()->second;

		if (bestcost > neighbours_wcost.begin()->first) {
			// aspiration criterion is met
			bestcost = neighbours_wcost.begin()->first;
			currentSolution = currentNeighbour.sudoku;

			nextSolutionFound = true;
		}
		else {
			// check if not tabu
			bool notTabu = true;

			if (isTabu(tabulist, currentNeighbour.swaps))
			{
				notTabu = false;
			}

			if (notTabu) {
				currentSolution = currentNeighbour.sudoku;
				nextSolutionFound = true;
			}
			else {
				neighbours_wcost.erase(neighbours_wcost.begin());
			}
		}
		if (neighbours_wcost.size() == 0) {
			Logger::getInstance().logError("Stuck during TabuSearch, number of neighbours with cost is 0.");
			break;
		}
	}
	return currentNeighbour;
}


TabuSearch::Neighbour TabuSearch::getNextSolutionByRandomWalk(Sudoku currentSolution) {
	int x, y;
	// Generate Random select
	int unfixedCells = currentSolution.getNumberOfUnfixedCells();

	int rand = Util::randomInt(0, unfixedCells-1);
	std::pair<int, int> xy = currentSolution.getUnfixedIndices()[rand];

	return TabuSearch::getNeighbour(currentSolution, xy.first, xy.second);
}

TabuSearch::Neighbour TabuSearch::getNextSolutionByMCWithTabulist(Sudoku currentSolution,
		std::vector<std::vector<std::vector<std::vector<int> > > >& tabulist, int tabulistSize, int bestCost, int , int iteration) {

	std::pair<int, int> xy = currentSolution.getRandomlyConflictedField();
	Swap bestSwapNotTabu;
	Swap bestSwapTabu;
	int nextBestCostNotTabu = 999999999;
	int nextBestCostTabu = 999999999;

	int x = xy.first;
	int y = xy.second;

	int order = currentSolution.getOrder();
	int swapx_offset = x - x % order;
	int swapy_offset = y - y % order;

	// generate iterators and shuffle them to randomly go through swap possibilities in random order
	std::vector<int> ivalues, jvalues;
	for (int i=0; i<order; i++) {
		ivalues.push_back(i);
		jvalues.push_back(i);
	}
	random_shuffle(ivalues.begin(), ivalues.end());
	random_shuffle(jvalues.begin(), jvalues.end());

	std::vector<std::pair<std::vector<Swap>, int> > candidates;

	// generate candidates
	for (std::vector<int>::iterator it = ivalues.begin(); it != ivalues.end(); ++it) {
		for (std::vector<int>::iterator jt = jvalues.begin(); jt != jvalues.end(); ++jt) {
			bool isFixed = currentSolution.isFixed(swapx_offset+*it, swapy_offset+*jt);
			if (!isFixed) { // Not locked and not same field as above

				// swap
				Swap swap;
				swap.x = x;
				swap.y = y;
				swap.swapx = swapx_offset+*it;
				swap.swapy = swapy_offset+*jt;
				Sudoku trySolution(currentSolution);

				int value1 = trySolution.getValue(swap.x, swap.y);
				int value2 = trySolution.getValue(swap.swapx, swap.swapy);

				trySolution.setValue(swap.x, swap.y, value2, true);
				trySolution.setValue(swap.swapx, swap.swapy, value1, true);

				int cost = trySolution.getCost();



				int tabu = tabulist[swap.x][swap.y][swap.swapx][swap.swapy];
				if (tabu != -1 && iteration-tabu < tabulistSize ) {
					if (cost < nextBestCostTabu) {
						nextBestCostTabu = cost;
						bestSwapTabu = swap;
					}
				}
				else {
					if (cost < nextBestCostNotTabu) {
						bestSwapNotTabu = swap;
						nextBestCostNotTabu = cost;
					}
				}
			}
		}
	}

	Swap bestSwap;
	if (nextBestCostTabu < nextBestCostNotTabu && nextBestCostTabu < bestCost) {
		bestSwap = bestSwapTabu;
	}
	else {
		if (nextBestCostNotTabu == 999999999) {
			bestSwap.x = x;
			bestSwap.y = y;
			bestSwap.swapx = x;
			bestSwap.swapy = y;
		}
		else {
			bestSwap = bestSwapNotTabu;
		}
	}

	int value1 = currentSolution.getValue(bestSwap.x, bestSwap.y);
	int value2 = currentSolution.getValue(bestSwap.swapx, bestSwap.swapy);

	currentSolution.setValue(bestSwap.x, bestSwap.y, value2);
	currentSolution.setValue(bestSwap.swapx, bestSwap.swapy, value1);
	Neighbour neighbour;


	neighbour.sudoku = currentSolution;
	std::vector<Swap> swaps;
	swaps.push_back(bestSwap);
	neighbour.swaps = swaps;
	neighbour.cost = currentSolution.getCost();


	return neighbour;
}

/** Generates all possible neighbours for a given cell. */
std::vector<TabuSearch::Neighbour> TabuSearch::getPossibleNeighbours(Sudoku sudoku, int x,
		int y) {
	std::vector<Neighbour> neighbours;

	Neighbour neighbour;
	Swap swap;
	swap.x = x;
	swap.y = y;

	int order = sudoku.getOrder();
	int swapx_offset = x - x % order;
	int swapy_offset = y - y % order;
	int swapx, swapy;

	for (int i=(x+1)%order; i<order; i++) {
		for (int j=(y+1)%order; j<order; j++) {
			neighbour.sudoku = sudoku;

			swapx = swapx_offset+i;
			swapy = swapy_offset+j;

			if (!sudoku.isFixed(swapx, swapy)) {

				int value1 = sudoku.getValue(x, y);
				int value2 = sudoku.getValue(swapx, swapy);

				neighbour.sudoku.setValue(x, y, value2);
				neighbour.sudoku.setValue(swapx, swapy, value1);
				swap.swapx = swapx;
				swap.swapy = swapy;
				neighbour.swaps.push_back(swap);
				neighbour.cost = neighbour.sudoku.getCost();

				neighbours.push_back(neighbour);
			}
		}
	}

	return neighbours;
}

std::pair<std::vector<Swap>, int> TabuSearch::findBestSwap(Sudoku sudoku, std::vector<std::pair<int, int> > xys, std::deque<std::vector<Swap> > tabulist, int bestCost, int currentCost, std::vector<Swap> swaps, int swapLimit) {
	if (swaps.size() == swapLimit) {
		for (int i=0; i < swaps.size(); i++) {
			// swap
			int value1 = sudoku.getValue(swaps[i].x, swaps[i].y);
			int value2 = sudoku.getValue(swaps[i].swapx, swaps[i].swapy);

			sudoku.setValue(swaps[i].x, swaps[i].y, value2, true);
			sudoku.setValue(swaps[i].swapx, swaps[i].swapy, value1, true);
		}

		int cost = sudoku.getCost();

		std::pair<std::vector<Swap>, int> result;
		result.first = swaps;
		result.second = cost;

		return result;
	}

	int currentLevel = swaps.size();
	int x = xys[currentLevel].first;
	int y = xys[currentLevel].second;

	int order = sudoku.getOrder();
	int swapx_offset = x - x % order;
	int swapy_offset = y - y % order;

	// generate iterators and shuffle them to randomly go through swap possibilities in random order
	std::vector<int> ivalues, jvalues;
	for (int i=0; i<order; i++) {
		ivalues.push_back(i);
		jvalues.push_back(i);
	}
	random_shuffle(ivalues.begin(), ivalues.end());
	random_shuffle(jvalues.begin(), jvalues.end());

	std::vector<std::pair<std::vector<Swap>, int> > candidates;

	// generate candidates
	for (std::vector<int>::iterator it = ivalues.begin(); it != ivalues.end(); ++it) {
		for (std::vector<int>::iterator jt = jvalues.begin(); jt != jvalues.end(); ++jt) {
			bool isFixed = sudoku.isFixed(swapx_offset+*it, swapy_offset+*jt);
			bool sameX = swapx_offset+*it == x;
			bool sameY = swapy_offset+*jt == y;
			if (!isFixed && !(sameX && sameY)) { // Not locked and not same field as above

				// swap
				Swap swap;
				swap.x = x;
				swap.y = y;
				swap.swapx = swapx_offset+*it;
				swap.swapy = swapy_offset+*jt;

				std::vector<Swap> new_swaps(swaps);
				new_swaps.push_back(swap);

				candidates.push_back(findBestSwap(sudoku, xys, tabulist, bestCost, currentCost, new_swaps, swapLimit));
			}
		}
	}

	int nextBestCost = 99999999;
	std::pair<std::vector<Swap>, int> bestCandidate;
	// select best candidate
	for (int i=0; i < candidates.size(); i++) {

		int new_cost = candidates[i].second;
		if (new_cost < bestCost && new_cost < nextBestCost) { // best fitness so far!
			bestCandidate = candidates[i];
		}
		else if (new_cost < nextBestCost && !isTabu(tabulist, candidates[i].first)) {
			nextBestCost = new_cost;
			bestCandidate = candidates[i];
		}
	}

	return bestCandidate;
}

std::vector<Swap> TabuSearch::minConflictsWithTabuList(Sudoku sudoku, std::vector<std::pair<int, int> > xys, std::deque<std::vector<Swap> > tabulist, int bestCost, int currentCost) {


	std::vector<Swap> bestSwaps;

	int nextBestCost = 99999999;


	for (int i=0; i<SWAP_COUNT; i++) {
		std::vector<Swap> swaps;
		std::pair<std::vector<Swap>, int> candidate;

		candidate = findBestSwap(sudoku, xys, tabulist, bestCost, currentCost, swaps, i+1);

		if (candidate.second < nextBestCost) {
			bestSwaps = candidate.first;
			nextBestCost = candidate.second;
		}
	}



	return bestSwaps;
}

Sudoku TabuSearch::tryCpApproach(Sudoku sudoku, float resets) {
	Logger::getInstance().logInfo("Entered tryCpApproach");

	std::vector< std::pair<int, int> > conflictedFields = sudoku.getConflictedFields();
	std::vector< std::pair<int, int> >::iterator it;

	std::stringstream ss;
	ss << "Number of conflicted fields: " << conflictedFields.size() << ", curcost: " << sudoku.getCost();

	Logger::getInstance().logInfo(ss.str());
	// Reset all conflicted Fields
	for (it = conflictedFields.begin(); it != conflictedFields.end(); it++) {
		sudoku.setValue(it->first, it->second, 0, false);
	}

	// Reset some more Fields randomly?
	if (resets == 1) {
		for (int i=0; i<sudoku.getUnfixedIndices().size(); i++) {
			sudoku.setValue(sudoku.getUnfixedIndices()[i].first, sudoku.getUnfixedIndices()[i].second, 0, false);
		}
	}
	else {
		int numrows = sudoku.getNumrows();
		int unfixedCells = sudoku.getNumberOfUnfixedCells();
		int x, y, calc_resets = unfixedCells * resets;



		std::stringstream ss2;
		ss2 << "calculated resets: " << calc_resets << " unfixed: " << unfixedCells;

		Logger::getInstance().logInfo(ss2.str());

		int i=0;


		int arr[unfixedCells];
		for(int i = 0; i < unfixedCells; i++) {
			arr[i] = i;
		}
		std::random_shuffle(arr, arr+unfixedCells);

		for (int i=0; i < calc_resets; i++) {
			std::pair<int, int> xy = sudoku.getUnfixedIndices()[arr[i]];
			sudoku.setValue(xy.first, xy.second, 0, false); // here it fails

		}
		Logger::getInstance().logInfo(sudoku.str());
	}

	// apply cpProcedure and see if solution is found
	sudoku.applyCpProcedure();
	sudoku.applyBtProcedure(BT_MINDOMAIN_FIRST, BT_HEURISTIC);
	Logger::getInstance().logInfo(sudoku.str());

	Logger::getInstance().logInfo("Returning from tryCpApproach");

	return sudoku;
}

void TabuSearch::printTabuList(std::deque<std::vector<Swap> > tabulist) {
	std::cout << "Printing tabulist:" << std::endl;

	for (int i = 0; i < tabulist.size(); i++) {
		std::cout << "==========" << std::endl;
		std::cout << "Tabulistentry Nr. " << i << std::endl;
		for (std::vector<Swap>::iterator it2 = tabulist[i].begin(); it2 != tabulist[i].end(); it2++) {
			std::cout << " swapx " << it2->swapx  << " swap y " << it2->swapy << " x " << it2->x << " y " << it2->y << std::endl;
		}
		std::cout << "==========" << std::endl;
	}
}

bool TabuSearch::isTabu(std::deque<std::vector<Swap> > tabulist, std::vector<Swap> swaps) {
	for (std::deque<std::vector<Swap> >::iterator it = tabulist.begin(); it != tabulist.end(); it++) {
		if (it->size() == swaps.size()) {
			bool match_all = true;
			std::vector<Swap>::iterator it2 = it->begin();
			std::vector<Swap>::iterator it3 = swaps.begin();
			while (it2 != it->end()) {
				if (!(it2->x == it3->x && it2->y == it3->y && it2->swapx == it3->swapx && it2->swapy == it3->swapy)
						&& !(it2->x == it3->swapx && it2->y == it3->swapy && it2->swapx == it3->x && it2->swapy == it3->y))
				{
					match_all = false;
					break;
				}
				it2++;
				it3++;
			}

			if (match_all) {
				return true;
			}
		}
	}

	return false;
}

