#!/bin/bash
# ./parallel.sh benchmarks/MOLS 1000 MOLS_ls
# ./parallel.sh benchmarks/MOLS 1000 MOLS_csp
# ./parallel.sh benchmarks/MOLS 1000 MOLS_cpl
# ./parallel.sh benchmarks/MOLS 1000 MOLS_choco
# ./parallel.sh benchmarks/MOLS 1000 MOLS_chuff
# ./parallel.sh benchmarks/MOLS 1000 MOLS_gec
# ./parallel.sh benchmarks/MOLS 1000 MOLS_ort 15

# ./parallel.sh benchmarks/N-Queens 1000 nqueens_ls
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_csp
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_cpl
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_gec
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_ort

# ./parallel.sh benchmarks/All-Interval 1000 AI_ls 10
# ./parallel.sh benchmarks/All-Interval 1000 AI_yuck
# ./parallel.sh benchmarks/All-Interval 1000 AI_cpl
# ./parallel.sh benchmarks/All-Interval 1000 AI_choco
# ./parallel.sh benchmarks/All-Interval 1000 AI_ort 5

benchmarks=$1
time_t=$2
solver=$3
times=$4

# function parallel() {
#     local seed=$1
#     nohup bash run.sh $benchmarks $time_t $solver $seed $ > results/$solver/$seed.log 2>&1 &
# }

function parallel() {
    local seed=$1
    local core=$(( (seed - 1) % $(nproc) ))  # 动态分配核心，假设核心从0开始，循环使用可用的核心
    nohup taskset -c $core bash run.sh $benchmarks $time_t $solver $seed > results/$solver/$seed.log 2>&1 &
}

for ((round = 10; round <= $times; round++)); do
    path=results/$solver
    if [ ! -d "$path" ];then
        mkdir -p $path
    fi
    parallel $round
done