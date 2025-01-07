#!/bin/bash
# ./parallel_des.sh proportion 1000 sudoku_des 1
# ./parallel_des.sh proportion_CP 1000 sudoku_choco_des 1
# ./parallel_des.sh proportion_CP 1000 sudoku_yuck_des 1
# ./parallel_des.sh proportion 1000 sudoku_cpl_des 1
benchmarks=$1
time_t=$2
solver=$3
seed=$4

for folder in $benchmarks/*;do
    foldername=`basename $folder`
    path=results/$solver
    if [ ! -d "$path" ];then
        mkdir -p $path
    fi
    nohup bash run.sh $folder $time_t $solver $seed > results/$solver/$foldername.log 2>&1 &
done