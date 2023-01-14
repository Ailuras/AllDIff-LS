/*
 * main.cpp
 *
 * Part of the Source Code for the Sudoku solver described in
 * "A Hybrid Heuristic Approach for the Sudoku Problem"
 */

#include "Sudoku.h"
#include "TabuSearch.h"
#include "IteratedLocalSearch.h"
#include "Util.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <getopt.h>

#include "Constants.h"

#include "../logger/Logger.h"

float TABULENGTH_RELATIVE;
float ALPHA;
int ACCEPTANCE_PROB;


int main(int argc, char **argv) {
	if (argc > 19 || argc < 9) {
		std::cout << "Usage: " << argv[0] << " " << "--instanceFile [instanceFile] --timeLimit [timeLimit] --mcProbability [mcProbability]"
				<< "--iterationLimit [iterationLimit] --tabuListSize [tabuListSize] --alpha [alpha] --acceptanceProb [acceptanceProb] --outfile [outfile] --logfile [logFile]" << std::endl;
		return -1;
	}

	Logger::getInstance().init();
	Util::seed(time(NULL));

	float tabuListSize;
	long timeLimit;
	long mcProbability;
	long iterationLimit;
	float alpha;
	long acceptanceProb;


	std::string outfile, log, instanceFile;
	outfile = "solution.txt";
	log = "log.txt";

	// getopt start

	int c;
	int digit_optind = 0;
	std::istringstream arg;



	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
				{"instanceFile",     required_argument, 0,  'i' },
				{"timeLimit",  required_argument, 0,  'l' },
				{"mcProbability", required_argument,       0,  'm' },
				{"iterationLimit", required_argument,       0,  'n' },
				{"tabuListSize", required_argument,       0,  'z' },
				{"alpha", required_argument,       0,  'a' },
				{"acceptanceProb", required_argument,       0,  'p' },
				{"outfile",  required_argument, 0, 'o'},
				{"logfile",    required_argument, 0,  's' },
				{0,         0,                 0,  0 }
		};

		c = getopt_long(argc, argv, "",
				long_options, &option_index);
		if (c == -1)
			break;
		arg.clear();

		switch (c) {
		case 'i':
			instanceFile = optarg;
			break;
		case 'l':
			arg.str(optarg);
			arg >> timeLimit;
			break;
		case 'm':
			arg.str(optarg);
			arg >> mcProbability;
			break;
		case 'n':
			arg.str(optarg);
			arg >> iterationLimit;
			break;
		case 'z':
			arg.str(optarg);
			arg >> tabuListSize;
			break;
		case 'a':
			arg.str(optarg);
			arg >> alpha;
			break;
		case 'p':
			arg.str(optarg);
			arg >> acceptanceProb;
			break;
		case 'o':
			outfile = optarg;
			break;
		case 's':
			log = optarg;
			break;

		default:
			printf("?? getopt returned character code 0%o ??\n", c);
		}
	}

	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}
	// getopt end


	TABULENGTH_RELATIVE = tabuListSize;
	ALPHA = alpha;
	ACCEPTANCE_PROB = acceptanceProb;

	timeLimit *= 1000;
	long starttime = Util::time();

	std::vector<Sudoku> sudokus = Sudoku::createSudokus(instanceFile);

	// Apply cp described in "On the Combination of Constraint Programming
	// and Stochastic Search: The Sudoku Case"
	sudokus[0].applyCpProcedure();
	sudokus[0].lockValues();


	// determine tabuListSize, it is dependent on the number of unfixed cells (60% of unfixed)
	int tabuLength = sudokus[0].getNumberOfUnfixedCells()*TABULENGTH_RELATIVE;

	Sudoku initial = TabuSearch::generateInitialSolution(sudokus[0]);
	Logger::getInstance().logInfo("Initial:");
	Logger::getInstance().logInfo(initial.str());


	Sudoku solution = IteratedLocalSearch::iteratedLocalSearch(initial, tabuLength, timeLimit, mcProbability, iterationLimit);
	long time = (Util::time()-starttime);
	Logger::getInstance().logInfo("Solution:");
	Logger::getInstance().logInfo(solution.str());
	Logger::getInstance().logInfo("Solution cost:");
	int finalCost = TabuSearch::evaluateSolution(solution);

	std::cout << finalCost << std::endl;
	// Logger::getInstance().logInfo("Time taken (msecs): ");
	// std::stringstream ss;
	// ss << time;
	// Logger::getInstance().logInfo(ss.str());

	// solution.printToFile(outfile);

	// std::ofstream logFile;
	// logFile.open(log.c_str(), std::fstream::app);
	// if (solution.getCost() == 0) {
	// 	logFile << 1 << " " << time << "\n";
	// }
	// else {
	// 	logFile << 0 << " " << time << "\n";
	// }
	// logFile.close();


	return 0;
}
