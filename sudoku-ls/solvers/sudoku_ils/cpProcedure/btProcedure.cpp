/*
 * btProcedure.cpp
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */


#include "btProcedure.h"
#include "cpProcedure.h"
#include <algorithm>
#include <set>
#include <vector>
#include <iostream>

#include "../logger/Logger.h"


std::vector<int> orderDomainValues(std::set<int> s, std::string heuristic) {
	std::vector<int> v( s.begin(), s.end() );

	if (heuristic == "SVal") {
		std::sort(v.begin(), v.end());
		return v;
	}
	else if (heuristic == "GVal") {
		std::sort(v.begin(), v.end());
		std::reverse(v.begin(), v.end());
		return v;
	}
	else if (heuristic == "AVal") {
		std::vector<int> v2;
		std::sort(v.begin(), v.end());

		while (v.size() > 1) {
			int avg = (int)(v.front()+v.back())/2;
			int bestvalue = -1;
			int bestI = -1;

			for (int i=0; i<v.size(); i++) {
				if (bestvalue < v[i] && v[i] <= avg) {
					bestI = i;
					bestvalue = v[i];
				}
			}

			v2.push_back(v[bestI]);
			v.erase(v.begin()+bestI);
		}

		v2.push_back(v.front());
		return v2;
	}
	else if (heuristic == "GAV") {
		std::vector<int> v2;
		std::sort(v.begin(), v.end());

		while (v.size() > 1) {
			int avg = (int)(v.front()+v.back())/2;
			int bestvalue = 9999;
			int bestI = -1;

			for (int i=0; i<v.size(); i++) {
				if (bestvalue > v[i] && v[i] > avg) {
					bestI = i;
					bestvalue = v[i];
				}
			}

			v2.push_back(v[bestI]);
			v.erase(v.begin()+bestI);
		}

		v2.push_back(v.front());
		return v2;
	}

	return v;
}


bool selectUnassignedVariable(int **grid, bool **fixed, int sqLen, int grLen, int nSq,
		int first, int last, bool minDomainFirst, std::string heuristic, clock_t finishTime) {
	std::vector< std::vector< std::set<int> > > cellLists;

	int importantCount;
	int importantI, importantJ;

	if (minDomainFirst) {
		importantCount = 9999;
	}
	else {
		importantCount = 0;
	}
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
			}
			else {
				std::set<int> empty;
				tmpLists.push_back(empty);
			}
			if (minDomainFirst && tmpLists[j-1].size() < importantCount && tmpLists[j-1].size() > 0) {
				importantCount = tmpLists[j-1].size();
				importantI = i;
				importantJ = j;
			}
			if (!minDomainFirst && tmpLists[j-1].size() > importantCount && tmpLists[j-1].size() > 0) {
				importantCount = tmpLists[j-1].size();
				importantI = i;
				importantJ = j;
			}
		}

		cellLists.push_back(tmpLists);
	}

	if (importantCount == 0 || importantCount == 9999) {
		return false;
	}
	std::vector<int> order = orderDomainValues(cellLists[importantI-1][importantJ-1], heuristic);


	int **newGrid = new int* [grLen];
	for(int j=0;j<grLen;j++) newGrid[j] = new int [grLen];
	bool **newFixed = new bool* [grLen];
	for(int j=0;j<grLen;j++) newFixed[j] = new bool [grLen];
	for (int i=0; i<order.size(); i++) {

		//copy oldGrid
		for(int r=0;r<grLen;r++){
			for(int c=0;c<grLen;c++){
				newGrid[r][c]=grid[r][c];
				newFixed[r][c]=fixed[r][c];
			}
		}

		newGrid[importantI-1][importantJ-1] = order[i];
		newFixed[importantI-1][importantJ-1] = true;

		cpProcedure(newGrid, newFixed, sqLen, grLen, nSq,
				first, last);

		if (btProcedure(newGrid, newFixed, sqLen, grLen, nSq, first,
				last, minDomainFirst, heuristic, finishTime)) {

			//copy newGrid
			for(int r=0;r<grLen;r++){
				for(int c=0;c<grLen;c++){
					grid[r][c]=newGrid[r][c];
					fixed[r][c]=newFixed[r][c];
				}
			}

			// delete  newGrid
			for(int r=0;r<grLen;r++){
				delete [] newGrid[r];
				delete [] newFixed[r];
			}
			delete [] newGrid;
			delete [] newFixed;

			return true;
		}
		else if (clock() >= finishTime) {
			return false;
		}

	}

	// delete  newGrid
	for(int r=0;r<grLen;r++){
		delete [] newGrid[r];
		delete [] newFixed[r];
	}
	delete [] newGrid;
	delete [] newFixed;


	return false;
}

bool btProcedure(int **grid, bool **fixed, int sqLen, int grLen, int nSq,
		int first, int last, bool minDomainFirst, std::string heuristic,
		clock_t finishTime) {

	if (clock() >= finishTime) {
		Logger::getInstance().logInfo("Out of time, stopping btProcedure");

		return false;
	}

	int unfixedCnt = 0;
	for(int r=0;r<grLen;r++){
		for(int c=0;c<grLen;c++){
			if(!fixed[r][c])unfixedCnt++;
		}
	}
	if (unfixedCnt<=0) {
		return true;
	}

	return selectUnassignedVariable(grid, fixed, sqLen, grLen, nSq, first,
			last, minDomainFirst, heuristic, finishTime);
}
