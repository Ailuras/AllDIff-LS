/*
 * Logger.h
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <sstream>

class Logger {

private:
	Logger() { loglevel = 0; };

	Logger(Logger const&);
	void operator=(Logger const&);
	int loglevel;
public:

	static Logger& getInstance()
	{
		static Logger instance;
		return instance;
	}

	void logInfo(std::string msg);
	void logWarn(std::string msg);
	void logError(std::string msg);
	void init();
};

#endif /* LOGGER_H_ */
