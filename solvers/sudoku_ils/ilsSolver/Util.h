/*
 * Util.h
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <time.h>
#include <stdlib.h>

class Util {
private:
	virtual ~Util();
	Util();
public:
	static long time();
	static void seed(unsigned int seed);
	static int randomInt(int lowerBoundary, int upperBoundary);
};

#endif /* UTIL_H_ */
