# CP path
export PATH=~/Sudoku-LS/solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/bin:$PATH;export LD_LIBRARY_PATH=~/Sudoku-LS/solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/lib:$LD_LIBRARY_PATH;export QT_PLUGIN_PATH=~/Sudoku-LS/solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/plugins:$QT_PLUGIN_PATH
# SAT solver
pip install z3-solver
# ACS solver
cd solvers/sudoku_acs/
make -f Makefile
cd ../..
# LSC solver
cd solvers/sudoku_lsc/
g++ sudoku_lsc.cpp -std=c++11 -O3 -o sudoku_lsc
g++ sudoku_new.cpp -std=c++11 -O3 -o sudoku_new
# g++ sudoku_test.cpp -std=c++11 -O3 -o sudoku_lsc
cd ../..
# ./parallel.sh benchmarks_hard 1000 sudoku_acs 1
# ./parallel.sh benchmarks_hard 1000 sudoku_lsc 1
# ./solvers/sudoku_acs/sudoku_acs --alg 0 --file  benchmarks_hard/INST_81x81/0/inst81x81_0_0.txt --timeout 100

minizinc -c --solver org.minizinc.mzn-fzn -I "/home/hanrui/Sudoku-LS/solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/share/minizinc/choco/mzn_lib" /home/hanrui/Sudoku-LS/solvers/sudoku_csp/MOLS.mzn -D "n=8" -o output.fzn
sh /home/hanrui/Sudoku-LS/solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/share/minizinc/choco/fzn-choco -jar /home/hanrui/Sudoku-LS/solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/share/minizinc/choco/choco-solver-4.10.13.jar [<options>] [<file>]


export PATH=solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/bin:$PATH;
export LD_LIBRARY_PATH=solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/lib:$LD_LIBRARY_PATH;
export QT_PLUGIN_PATH=solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/plugins:$QT_PLUGIN_PATH