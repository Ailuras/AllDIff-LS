#!/bin/bash
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
    if [ "$solver"x = "nqueens_ls_restart"x ]; then
        ./solvers/nqueens_ls_restart/NQueen-LS $file $seed $time_t
    fi
    if [ "$solver"x = "nqueens_ls_step"x ]; then
        ./solvers/nqueens_ls_step/NQueen-LS $file $seed $time_t
    fi
    if [ "$solver"x = "nqueens_csp"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        timeout $time_t minizinc --solver yuck solvers/nqueens_csp/nqueens.mzn -D "n=$number"
    fi
    if [ "$solver"x = "nqueens_choco"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        timeout $time_t minizinc --solver choco solvers/nqueens_csp/nqueens.mzn -D "n=$number"
    fi
    if [ "$solver"x = "nqueens_chuff"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        timeout $time_t minizinc --solver chuffed solvers/nqueens_csp/nqueens.mzn -D "n=$number"
    fi
    if [ "$solver"x = "nqueens_cpl"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        timeout $time_t python solvers/nqueens_cpl/nqueens.py $number
    fi

    if [ "$solver"x = "MOLS_ls"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        ./solvers/MOLS_ls/GLS-LS $number $seed $time_t
    fi
    if [ "$solver"x = "MOLS_ls_restart"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        ./solvers/MOLS_ls_restart/GLS-LS $number $seed $time_t
    fi    
    if [ "$solver"x = "MOLS_ls_step"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        ./solvers/MOLS_ls_step/GLS-LS $number $seed $time_t
    fi
    if [ "$solver"x = "MOLS_csp"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        minizinc --solver yuck --time-limit $[$time_t * 1000] solvers/MOLS_csp/mols.mzn -D "n=$number"
    fi
    if [ "$solver"x = "MOLS_choco"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        minizinc --solver choco --time-limit $[$time_t * 1000] solvers/MOLS_csp/mols.mzn -D "n=$number"
    fi
    if [ "$solver"x = "MOLS_chuff"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        minizinc --solver chuffed --time-limit $[$time_t * 1000] solvers/MOLS_csp/mols.mzn -D "n=$number"
    fi
    if [ "$solver"x = "MOLS_cpl"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        timeout $time_t python solvers/MOLS_cpl/mols.py $number
    fi

    if [ "$solver"x = "AI_ls"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        ./solvers/AI_ls/AI-LS $number $seed $time_t
    fi
    if [ "$solver"x = "AI_choco"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        timeout $time_t minizinc --solver choco solvers/AI_csp/allinterval.mzn -D "n=$number"
    fi
    if [ "$solver"x = "AI_cpl"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        timeout $time_t python solvers/AI_cpl/allinterval.py $number
    fi
    if [ "$solver"x = "AI_yuck"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        timeout $time_t minizinc --solver yuck solvers/AI_csp/allinterval.mzn -D "n=$number"
    fi

    end=$[$(date +%s%N)/1000000]
    take=$(( end - start ))
    echo $file : ${take} ms.
done