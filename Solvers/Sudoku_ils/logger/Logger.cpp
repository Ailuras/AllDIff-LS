/*
 * Logger.cpp
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#include "Logger.h"
#include <iostream>

void Logger::logInfo(std::string msg) {
	if (loglevel <= 0) {
		std::cout << msg << std::endl;
	}
}

void Logger::logWarn(std::string msg) {
	if (Logger::loglevel <= 1) {
		std::cout << "[WARNING] " << msg << std::endl;
	}
}

void Logger::logError(std::string msg) {
	if (loglevel <= 2) {
		std::cout << "[ERROR] "  << msg << std::endl;
	}
}

void Logger::init() {
	// set level statically INFO=0, WARNING=1, ERROR=2, 3 will disable logging
	loglevel=3;
}
