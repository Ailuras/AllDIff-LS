/*
 * cpProcedure.cpp
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */


#include "cpProcedure.h"
#include <algorithm>
#include <vector>
#include <set>


/**
 * Applies the cpProcedure as described in 'On the Combination of Constraint
 * Programming and Stochastic Search: The Sudoku Case'
 *
 * sqLen: the length of a square
 * grLen: the length of the grid
 * nSq: the number of squares per row/col
 * first: the lowest possible value
 * last: the greatest possible cell value
 */
void cpProcedure(int **grid, bool **fixed, int sqLen, int grLen, int nSq,
		int first, int last) {
	bool end = false;

	while (!end) {
		end = executionLoop(grid, fixed, sqLen, grLen, nSq, first, last);
	}
}

std::set<int> getListWithPossibleValues(int **grid, bool **fixed, int sqLen,
		int first, int last, int x, int y) {

	std::set<int> referenceSet, actualSet;
	std::set<int> setdiff;

	if (fixed[x-1][y-1]) {
		return setdiff;
	}

	for (int i=first; i<last; i++) {
		referenceSet.insert(i);
	}

	// insert row values
	for (int i=first; i<last; i++) {
		if (fixed[i-1][y-1])
			actualSet.insert(grid[i-1][y-1]);
	}

	// insert column values
	for (int i=first; i<last; i++) {
		if (fixed[x-1][i-1])
			actualSet.insert(grid[x-1][i-1]);
	}

	// insert square values
	int xval = ((x-1)/sqLen)*sqLen+1;
	int yval = ((y-1)/sqLen)*sqLen+1;

	for (int i=xval; i<xval+sqLen; i++) {
		for (int j=yval; j<yval+sqLen; j++) {
			if (fixed[i-1][j-1])
				actualSet.insert(grid[i-1][j-1]);
		}
	}

	std::set_difference(referenceSet.begin(), referenceSet.end(),
			actualSet.begin(), actualSet.end(),
			std::inserter(setdiff, setdiff.end()));

	return setdiff;
}

void checkForOnlyOnePossibleValue(int **grid, bool **fixed, int sqLen, int grLen,
		int nSq, int first, int last, int x, int y, std::set<int> setdiff) {

	if (setdiff.size() == 1) {
		fixed[x-1][y-1] = true;
		grid[x-1][y-1] = *setdiff.begin();
	}
}

int* checkForNoOtherPossiblePlace(std::vector< std::set<int > > cellLists, int first, int last) {
	for (int i=first; i<last; i++) {
		int count = 0;
		int lastfound = -1;

		for (int j=first; j<last; j++) {
			if (cellLists[j-1].find(i) != cellLists[j-1].end()) {
				count++;
				lastfound = j;
				if (count == 2) {
					break;
				}
			}
		}

		if (count == 1) {
			int *val = new int[2];
			val[0] = i;
			val[1] = lastfound;
			return val;
		}
	}

	return 0;
}

bool checkForRows(int **grid, bool **fixed, int sqLen, int grLen,
		int nSq, int first, int last,
		std::vector< std::vector< std::set<int> > > allLists) {
	for (int i=first; i<last; i++) {
		std::vector< std::set<int> > cellLists;

		for (int j=first; j<last; j++) {
			cellLists.push_back(allLists[i-1][j-1]);
		}
		int *val = checkForNoOtherPossiblePlace(cellLists, first, last);

		if (val != 0) {
			grid[i-1][val[1]-1] = val[0];
			fixed[i-1][val[1]-1] = true;

			delete [] val;

			return false;
		}
	}
	return true;
}

bool checkForCols(int **grid, bool **fixed, int sqLen, int grLen,
		int nSq, int first, int last,
		std::vector< std::vector< std::set<int> > > allLists) {
	for (int i=first; i<last; i++) {
		std::vector< std::set<int> > cellLists;

		for (int j=first; j<last; j++) {
			cellLists.push_back(allLists[j-1][i-1]);
		}

		int *val = checkForNoOtherPossiblePlace(cellLists, first, last);

		if (val != 0) {
			grid[val[1]-1][i-1] = val[0];
			fixed[val[1]-1][i-1] = true;

			delete [] val;

			return false;
		}
	}
	return true;
}

bool checkForSquares(int **grid, bool **fixed, int sqLen, int grLen,
		int nSq, int first, int last,
		std::vector< std::vector< std::set<int> > > allLists) {
	for (int i=first; i<last; i++) {
		std::vector< std::set<int> > cellLists;

		int jval = ((i-1)/sqLen)*sqLen+1;
		int kval = ((i-1)%sqLen)*sqLen+1;
		for (int j=jval; j<jval+sqLen; j++) {
			for (int k=kval; k<kval+sqLen; k++) {
				cellLists.push_back(allLists[j-1][k-1]);
			}
		}

		int *val = checkForNoOtherPossiblePlace(cellLists, first, last);

		if (val != 0) {

            int xcoord = jval-1+((val[1]-1)/sqLen+1)-1;
            int ycoord = kval-1+((val[1]-1)%sqLen+1)-1;

            grid[xcoord][ycoord] = val[0];
			fixed[xcoord][ycoord] = true;

			delete [] val;

			return false;
		}
	}
	return true;
}

bool executionLoop(int **grid, bool **fixed, int sqLen, int grLen, int nSq,
		int first, int last) {

	std::vector< std::vector< std::set<int> > > cellLists;

	/**
	 * For each unfixed cell in the grid, construct a list of possible values
	 * that this cell could contain by examining the contents of the cell's row,
	 * column, and box;
	 */
	for (int i=first; i<last; i++) {
		std::vector< std::set<int> > tmpLists;

		for (int j=first; j<last; j++) {
			if (!fixed[i-1][j-1]) {
				tmpLists.push_back(getListWithPossibleValues(grid, fixed, sqLen,
							first, last, i, j));

				checkForOnlyOnePossibleValue(grid, fixed, sqLen, grLen, nSq,
						first, last, i, j, tmpLists[j-1]);

				if (fixed[i-1][j-1]) {
					return false;
				}
			}
			else {
				std::set<int> empty;
				tmpLists.push_back(empty);
			}
		}
		cellLists.push_back(tmpLists);
	}

	if (!checkForRows(grid, fixed, sqLen, grLen, nSq, first, last, cellLists)) {
		return false;
	}
	if (!checkForCols(grid, fixed, sqLen, grLen, nSq, first, last, cellLists)) {
		return false;
	}
	if (!checkForSquares(grid, fixed, sqLen, grLen, nSq, first, last, cellLists)) {
		return false;
	}


	return true;
}

