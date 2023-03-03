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
    if [ "$solver"x = "nqueens_ls"x ]; then
        ./solvers/nqueens_ls/NQueen-LS $file $seed $time_t
    fi
    if [ "$solver"x = "nqueens_ort"x ]; then
        timeout $time_t python solvers/nqueens_ort/n-queens_ortools.py $file
    fi
    if [ "$solver"x = "nqueens_sat"x ]; then
        timeout $time_t ./solvers/nqueens_sat/Kissat_MAB-HyWalk/kissat -n -q $file
    fi
    end=$[$(date +%s%N)/1000000]
    take=$(( end - start ))
    echo $file : ${take} ms.
done