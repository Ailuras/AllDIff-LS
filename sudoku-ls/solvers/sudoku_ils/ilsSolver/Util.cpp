/*
 * Util.cpp
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#include "Util.h"
#include <iostream>
#include <ctime>

Util::~Util() {
}

Util::Util() {
}

long Util::time() {
	return 1000*(clock()/(double) CLOCKS_PER_SEC);
}

void Util::seed(unsigned int seed) {
	srand(seed);
}

int Util::randomInt(int lowerBoundary, int upperBoundary) {
	int values = upperBoundary-lowerBoundary;

	return lowerBoundary + rand()%(values+1);
}
