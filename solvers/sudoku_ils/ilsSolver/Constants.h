
/*
 * Constants.h
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>

extern int ACCEPTANCE_PROB;

extern float ALPHA;

extern float TABULENGTH_RELATIVE;

const int BT_TIMELIMIT = 5;
const bool BT_MINDOMAIN_FIRST = true;
const std::string BT_HEURISTIC = "SVal";


const int SWAP_COUNT = 1;

#endif /* CONSTANTS_H_ */
