#！/bin/bash
# ./solvers/sudoku_acs/sudoku_acs --alg 0 --file test.txt --timeout 1000


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
        timeout $time_t ./solvers/sudoku_lsc/sudoku_lsc $file $seed $time_t
    fi
    end=$[$(date +%s%N)/1000000]
    take=$(( end - start ))
    echo $file : ${take} ms.
done