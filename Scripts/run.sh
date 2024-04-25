#！/bin/bash

folder=$1
time_t=$2
solver=$3
seed=$4

for file in $folder/*; do
    echo --------------------------------------------------
    start=$[$(date +%s%N)/1000000]
    if [ "$solver"x = "sudoku_acs"x ]; then
        ../Solvers/Sudoku_acs/sudoku_acs --alg 0 --file $file --timeout $time_t
    fi
    if [ "$solver"x = "sudoku_ils"x ]; then
        ../solvers/Sudoku_ils/build/ilsSolver --instanceFile $file --timeLimit $time_t --mcProbability 100 --iterationLimit 1000 --tabuListSize 0.05 --alpha 0.8 --acceptanceProb 15 --outfile output.txt --logfile log.txt
    fi

    if [ "$solver"x = "sudoku_ortools"x ]; then
        timeout $time_t python ../Solvers/OR-Tools/sudoku.py $file
    fi
    if [ "$solver"x = "nqueens_ortools"x ]; then
        timeout $time_t python ../Solvers/OR-Tools/nqueens.py $file
    fi
    if [ "$solver"x = "allinterval_ortools"x ]; then
        timeout $time_t python ../Solvers/OR-Tools/allinterval.py $file
    fi
    if [ "$solver"x = "mols_ortools"x ]; then
        timeout $time_t python ../Solvers/OR-Tools/mols.py $file
    fi

    if [ "$solver"x = "sudoku_cplex"x ]; then
        timeout $time_t python ../Solvers/CPLEX/sudoku.py $file
    fi
    if [ "$solver"x = "nqueens_cplex"x ]; then
        timeout $time_t python ../Solvers/CPLEX/nqueens.py $file
    fi
    if [ "$solver"x = "allinterval_cplex"x ]; then
        timeout $time_t python ../Solvers/CPLEX/allinterval.py $file
    fi
    if [ "$solver"x = "mols_cplex"x ]; then
        timeout $time_t python ../Solvers/CPLEX/mols.py $file
    fi

    if [ "$solver"x = "sudoku_choco"x ]; then
        minizinc --solver choco --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/sudoku.mzn $file
    fi
    if [ "$solver"x = "sudoku_yuck"x ]; then
        minizinc --solver yuck --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/sudoku.mzn $file
    fi
    if [ "$solver"x = "sudoku_gecode"x ]; then
        minizinc --solver gecode --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/sudoku.mzn $file
    fi
    if [ "$solver"x = "nqueens_choco"x ]; then
        minizinc --solver choco --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/nqueens.mzn $file
    fi
    if [ "$solver"x = "nqueens_yuck"x ]; then
        minizinc --solver yuck --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/nqueens.mzn $file
    fi
    if [ "$solver"x = "nqueens_gecode"x ]; then
        minizinc --solver gecode --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/nqueens.mzn $file
    fi
    if [ "$solver"x = "allinterval_choco"x ]; then
        minizinc --solver choco --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/allinterval.mzn $file
    fi
    if [ "$solver"x = "allinterval_yuck"x ]; then
        minizinc --solver yuck --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/allinterval.mzn $file
    fi
    if [ "$solver"x = "allinterval_gecode"x ]; then
        minizinc --solver gecode --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/allinterval.mzn $file
    fi
    if [ "$solver"x = "mols_choco"x ]; then
        minizinc --solver choco --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/mols.mzn $file
    fi
    if [ "$solver"x = "mols_yuck"x ]; then
        minizinc --solver yuck --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/mols.mzn $file
    fi
    if [ "$solver"x = "mols_gecode"x ]; then
        minizinc --solver gecode --time-limit $[$time_t * 1000] ../Solvers/MiniZinc/mols.mzn $file
    fi

    if [ "$solver"x = "sudoku_kissat"x ]; then
        timeout $time_t ../Solvers/Kissat/Kissat/kissat -n -q $file
    fi

    if [ "$solver"x = "sudoku_AllDiff-LS"x ]; then
        ../Solvers/AllDiff-LS/Sudoku-LS $file $seed $time_t 0
    fi
    if [ "$solver"x = "nqueens_AllDiff-LS"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        ../Solvers/AllDiff-LS/NQueen-LS $number $seed $time_t
    fi
    if [ "$solver"x = "allinterval_AllDiff-LS"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        ../Solvers/AllDiff-LS/AI-LS $number $seed $time_t
    fi
    if [ "$solver"x = "mols_AllDiff-LS"x ]; then
        number=$(echo $file | grep -o -E '[0-9]+')
        ../Solvers/AllDiff-LS/MOLS-LS $number $seed $time_t
    fi
    if [ "$solver"x = "test"x ]; then
        echo $file
    fi
    end=$[$(date +%s%N)/1000000]
    take=$(( end - start ))
    echo $file : ${take} ms.
done