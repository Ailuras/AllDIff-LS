#！/bin/bash
# ./solvers/sudoku_acs/sudoku_acs --alg 0 --file test.txt --timeout 1000
# ./solvers/sudoku_lsc/sudoku_lsc benchmarks/INST_49x49/0/inst49x49_0_0.txt 1 1000


folder=$1
time_t=$2
solver=$3
seed=$4

for file in $folder/*; do
    echo --------------------------------------------------
    start=$[$(date +%s%N)/1000000]
    if [ "$solver"x = "sudoku_acs"x ]; then
        ./solvers/sudoku_acs/sudoku_acs --alg 0 --file $file --timeout $time_t
    fi
    if [ "$solver"x = "sudoku_lsc"x ]; then
        ./solvers/sudoku_lsc/sudoku_lsc $file $seed $time_t
    fi
    if [ "$solver"x = "sudoku_csp"x ]; then
        ./solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/bin/minizinc --solver gecode --time-limit $[$time_t * 1000] ./solvers/sudoku_csp/sudoku.mzn $file
    fi
    if [ "$solver"x = "sudoku_csp"x ]; then
        python solvers/sudoku_sat/generator.py --file $file
        timeout $time_t ./solvers/sudoku_sat/z3  $file
    fi
    end=$[$(date +%s%N)/1000000]
    take=$(( end - start ))
    echo $file : ${take} ms.
done