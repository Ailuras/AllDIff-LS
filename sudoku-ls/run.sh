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
    if [ "$solver"x = "sudoku_ils"x ]; then
        ./solvers/sudoku_ils/build/ilsSolver --instanceFile $file --timeLimit $time_t --mcProbability 100 --iterationLimit 1000 --tabuListSize 0.05 --alpha 0.8 --acceptanceProb 15 --outfile output.txt --logfile log.txt
    fi
    if [ "$solver"x = "sudoku_ort"x ]; then
        timeout $time_t python solvers/sudoku_ort/sudoku.py $file
    fi
    # dyh: 2024/4/20
    if [ "$solver"x = "sudoku_eq_ort"x ]; then
        timeout $time_t python solvers/sudoku_ort/sudoku_dyh_eq.py $file
    fi
    # dyh: 2024/4/21
    if [ "$solver"x = "sudoku_leq_ort"x ]; then
        timeout $time_t python solvers/sudoku_ort/sudoku_dyh_leq.py $file
    fi        
    if [ "$solver"x = "sudoku_eq"x ]; then
        ./solvers/Sudoku/Sudoku-LS-eq $file $seed $time_t 0
    fi
    if [ "$solver"x = "sudoku_lt"x ]; then
        ./solvers/Sudoku/Sudoku-LS-lt $file $seed $time_t 0
    fi   
    if [ "$solver"x = "sudoku_gac"x ]; then
        timeout $time_t python ./solvers/Sudoku/Sudoku_GAC.py $file
    fi
    if [ "$solver"x = "sudoku_ort9x9"x ]; then
        timeout $time_t python solvers/sudoku_ort/sudoku_9x9.py $file
    fi
    if [ "$solver"x = "sudoku_csp"x ]; then
        minizinc --solver or-tools --time-limit $[$time_t * 1000] ./solvers/sudoku_csp/sudoku.mzn $file
        # ./solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/bin/minizinc --solver gecode --time-limit $[$time_t * 1000] ./solvers/sudoku_csp/sudoku.mzn $file
    fi
    if [ "$solver"x = "sudoku_choco"x ]; then
        minizinc --solver choco --time-limit $[$time_t * 1000] ./solvers/sudoku_csp/sudoku.mzn $file
        # ./solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/bin/minizinc --solver gecode --time-limit $[$time_t * 1000] ./solvers/sudoku_csp/sudoku.mzn $file
    fi
    if [ "$solver"x = "sudoku_gec"x ]; then
        minizinc --solver gecode --time-limit $[$time_t * 1000] ./solvers/sudoku_gec/sudoku.mzn $file
        # ./solvers/sudoku_csp/MiniZincIDE-2.6.4-bundle-linux-x86_64/bin/minizinc --solver gecode --time-limit $[$time_t * 1000] ./solvers/sudoku_csp/sudoku.mzn $file
    fi
    if [ "$solver"x = "sudoku_sat"x ]; then
        timeout $time_t ./solvers/sudoku_sat/Kissat_MAB-HyWalk/kissat -n -q $file
    fi
    if [ "$solver"x = "sudoku_lsc"x ]; then
        ./solvers/sudoku_lsc/sudoku_lsc $file $seed $time_t
    fi
    if [ "$solver"x = "sudoku_ls"x ]; then
        ./solvers/Sudoku/Sudoku-LS $file $seed $time_t 0
    fi
    if [ "$solver"x = "sudoku_ls00"x ]; then
        ./solvers/Sudoku/Sudoku-LS_00 $file $seed $time_t 0
    fi
    if [ "$solver"x = "sudoku_ls03"x ]; then
        ./solvers/Sudoku/Sudoku-LS_03 $file $seed $time_t 0
    fi
    if [ "$solver"x = "sudoku_ls09"x ]; then
        ./solvers/Sudoku/Sudoku-LS_09 $file $seed $time_t 0
    fi
    if [ "$solver"x = "sudoku_ls12"x ]; then
        ./solvers/Sudoku/Sudoku-LS_12 $file $seed $time_t 0
    fi
    if [ "$solver"x = "sudoku_ls15"x ]; then
        ./solvers/Sudoku/Sudoku-LS_15 $file $seed $time_t 0
    fi
    if [ "$solver"x = "sudoku_tie"x ]; then
        ./solvers/sudoku_test_tie/sudoku_ls $file $seed $time_t 0
    fi
    if [ "$solver"x = "sudoku_tabu"x ]; then
        ./solvers/sudoku_test_tabu/sudoku_ls $file $seed $time_t
    fi
    if [ "$solver"x = "sudoku_swap"x ]; then
        ./solvers/sudoku_test_swap/sudoku_ls $file $seed $time_t
    fi
    if [ "$solver"x = "sudoku_cpl"x ]; then
        timeout $time_t python solvers/sudoku_cpl/sudoku.py $file
    fi
    if [ "$solver"x = "sudoku_cpltest"x ]; then
        timeout $time_t python solvers/sudoku_cpl/sudoku.py $file
    fi
    if [ "$solver"x = "sudoku_des"x ]; then
        ./solvers/sudoku_des/sudoku_lsc solvers/sudoku_des/inst64x64_0_0.txt $file $seed $time_t
    fi
    if [ "$solver"x = "sudoku_choco_des"x ]; then
        minizinc --solver choco --time-limit $[$time_t * 1000] ./solvers/sudoku_csp/sudoku_prop.mzn ./solvers/sudoku_csp/inst49x49_0_0.dzn $file
    fi
    if [ "$solver"x = "sudoku_yuck_des"x ]; then
        minizinc --solver yuck --time-limit $[$time_t * 1000] ./solvers/sudoku_csp/sudoku_prop.mzn ./solvers/sudoku_csp/inst49x49_0_0.dzn $file
    fi
    if [ "$solver"x = "sudoku_cpl_des"x ]; then
        timeout $time_t python solvers/sudoku_cpl/sudoku_prop.py solvers/sudoku_cpl/inst49x49_0_0.txt $file
    fi
    if [ "$solver"x = "sudoku_rule1"x ]; then
        ./solvers/sudoku_rule1/sudoku_lsc $file $seed $time_t
    fi
    if [ "$solver"x = "sudoku_rule234"x ]; then
        ./solvers/sudoku_rule234/sudoku_lsc $file $seed $time_t
    fi
    if [ "$solver"x = "sudoku_rule1234"x ]; then
        ./solvers/sudoku_rule1234/sudoku_lsc $file $seed $time_t
    fi
    end=$[$(date +%s%N)/1000000]
    take=$(( end - start ))
    echo $file : ${take} ms.
done

# nohup bash run.sh proportion_CP 1000 sudoku_choco_des > results/sudoku_choco_des/output1.log 2>&1 &