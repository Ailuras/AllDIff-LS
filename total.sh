# LS solver
cd solvers/nqueens_ls/
# g++ sudoku_lsc.cpp -std=c++11 -O3 -o sudoku_lsc
g++ NQueen-LS.cpp -std=c++11 -O3 -o NQueen-LS
# g++ NQueen-gen.cpp -std=c++11 -O3 -o NQueen-gen
# g++ sudoku_test.cpp -std=c++11 -O3 -o sudoku_lsc
cd ../..
# ./parallel.sh benchmarks_hard 1000 sudoku_acs 1
# ./parallel.sh benchmarks_hard 1000 sudoku_lsc 1
# ./solvers/sudoku_acs/sudoku_acs --alg 0 --file  benchmarks_hard/INST_81x81/0/inst81x81_0_0.txt --timeout 100
