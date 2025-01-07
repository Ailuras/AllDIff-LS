# README for the Source Code of the Sudoku solver described in "A Hybrid Heuristic Approach for the Sudoku Problem"

All the source files are written in C++ and can be compiled on Linux using the
provided Makefile and gcc.

To build all the required sources, simply type 'make'.
After the sources have been compiled, change into the build directory and start
the 'ilsSolver' program with its required parameters.

To call the solver with the parameters described in "A Hybrid Heuristic Approach for the Sudoku Problem" use:
./ilsSolver --instanceFile path/to/puzzle1.txt --timeLimit 5 --mcProbability 100 --iterationLimit 1000 --tabuListSize 0.05 --alpha 0.8 --acceptanceProb 15 --outfile output.txt --logfile log.txt

Short description of the parameters:
* instanceFile: A file which contains a Sudoku puzzle instance
* timeLimit: Overall timelimit for the Sudoku Solver in seconds
* mcProbability: The probability for using the MinConflicts heuristic in a 
  Local Search step (between 0 and 100). If MinConflicts is not chosen 
  by chance, the search will perform a Random Walk
* iterationLimit: The maximum number of iterations the embedded Local Search
  will perform without improvement in cost
* tabuListSize: The relative amount of unfixed cells in the given puzzle
  determines the tabu list length (between 0.0 and 1.0)
* alpha: The cooldown rate of the resetFactor which is applied during the 
  perturbation phase in Iterated Local Search (between 0.0 and 1.0)
* acceptanceProb: A neighbor which will bring no cost improvement is accepted 
  with this chance during Local Search (between 0 and 100)
* outfile: The acquired solution will be written to this file.
* logfile: Logging information will be written to this file.