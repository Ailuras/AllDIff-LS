g++ sudoku_lsc_re.cpp -std=c++11 -O3 -o sudoku_lsc_re
./sudoku_lsc_re inst49x49_50_0.txt 2 50 > re.log

g++ sudoku_lsc_new.cpp -std=c++11 -O3 -o sudoku_lsc_new
./sudoku_lsc_new inst49x49_50_0.txt 2 50 > new.log