cd solvers/sudoku_acs/
make -f Makefile
cd ../..
cd solver/sudoku_lsc/
g++ sudoku_lsc.cpp -std=c++11 -O3 -o sudoku_lsc
cd ../..
./parallel.sh benchmarks 1000 sudoku_acs 1
./parallel.sh benchmarks 1000 sudoku_lsc 1