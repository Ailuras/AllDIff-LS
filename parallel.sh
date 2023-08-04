#!/bin/bash
# ./parallel.sh benchmarks/MOLS 1000 MOLS_ls
# ./parallel.sh benchmarks/MOLS 1000 MOLS_csp
# ./parallel.sh benchmarks/MOLS 1000 MOLS_cpl
# ./parallel.sh benchmarks/MOLS 1000 MOLS_choco
# ./parallel.sh benchmarks/MOLS 1000 MOLS_chuff
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_ls
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_csp
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_cpl
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_gec
# ./parallel.sh benchmarks/MOLS 1000 MOLS_gec

# ./parallel.sh benchmarks/N-Queens 1000 nqueens_ls
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_csp
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_cpl
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_gec

benchmarks=$1
time_t=$2
solver=$3

function parallel() {
    local seed=$1
    nohup bash run.sh $benchmarks $time_t $solver $seed $ > results/$solver/$seed.log 2>&1 &
}

for ((round = 1; round <= 1; round++)); do
    path=results/$solver
    if [ ! -d "$path" ];then
        mkdir -p $path
    fi
    parallel $round
done