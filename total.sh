# LS solver
cd solvers/nqueens_ls/
# g++ sudoku_lsc.cpp -std=c++11 -O3 -o sudoku_lsc
g++ NQueen-LS.cpp -std=c++11 -O3 -o NQueen-LS
g++ GLS-LS.cpp -std=c++11 -O3 -o GLS-LS
# g++ NQueen-gen.cpp -std=c++11 -O3 -o NQueen-gen
# g++ sudoku_test.cpp -std=c++11 -O3 -o sudoku_lsc
cd ../..
# ./parallel.sh benchmarks_hard 1000 sudoku_acs 1
# ./parallel.sh benchmarks_hard 1000 sudoku_lsc 1
# ./solvers/sudoku_acs/sudoku_acs --alg 0 --file  benchmarks_hard/INST_81x81/0/inst81x81_0_0.txt --timeout 100
# export PATH=solvers/latinSquare_yur/MiniZincIDE-2.6.4-bundle-linux-x86_64/bin:$PATH
# export LD_LIBRARY_PATH=solvers/latinSquare_yur/MiniZincIDE-2.6.4-bundle-linux-x86_64/lib:$LD_LIBRARY_PATH
# export QT_PLUGIN_PATH=solvers/latinSquare_yur/MiniZincIDE-2.6.4-bundle-linux-x86_64/plugins:$QT_PLUGIN_PATH

export PATH=./MiniZincIDE-2.7.2-bundle-linux-x86_64/bin:$PATH
export LD_LIBRARY_PATH=./MiniZincIDE-2.7.2-bundle-linux-x86_64/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=./MiniZincIDE-2.7.2-bundle-linux-x86_64/plugins:$QT_PLUGIN_PATH

# export PATH=solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/bin:$PATH
# export LD_LIBRARY_PATH=solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/lib:$LD_LIBRARY_PATH
# export QT_PLUGIN_PATH=solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/plugins:$QT_PLUGIN_PATH

minizinc --solver yuck --time-limit 60 ./solvers/sudoku_csp/MOLS.mzn -D "n=4"
minizinc --solver gecode --time-limit 60 ./solvers/MOLS_csp/mols.mzn -D "n=4"