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
    if [ "$solver"x = "sudoku_ort"x ]; then
        timeout $time_t python solvers/sudoku_ort/sudoku.py $file
    fi
    if [ "$solver"x = "sudoku_csp"x ]; then
        minizinc --solver or-tools --time-limit $[$time_t * 1000] ./solvers/sudoku_csp/sudoku.mzn $file
        # ./solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/bin/minizinc --solver gecode --time-limit $[$time_t * 1000] ./solvers/sudoku_csp/sudoku.mzn $file
    fi
    if [ "$solver"x = "sudoku_sat"x ]; then
        timeout $time_t ./solvers/sudoku_sat/Kissat_MAB-HyWalk/kissat -n -q $file
    fi
    end=$[$(date +%s%N)/1000000]
    take=$(( end - start ))
    echo $file : ${take} ms.
done
    Sudoku problem is an important and challenging constraint satisfaction problem, which has numerous applications. Considering the difficulties for existing approaches in solving large-scale Sudoku problems, we propose a novel local search algorithm called Sudoku-LS in this work. Firstly, we use Sudoku graph to represent the problem, and refine the Sudoku graph through four simplification rules. Secondly, we propose an efficient two-step move selection method, and two heuristics based on the neighbor and global information, respectively, to break the ties. Finally, we introduce a new color forbidding strategy and a new weight restart strategy to improve the search efficiency. Experiments show that our proposed algorithm performs better than other state-of-art approaches, and also analyze the effectiveness of the proposed strategies.