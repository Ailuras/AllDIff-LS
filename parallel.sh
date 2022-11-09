#！/bin/bash
# ./parallel.sh benchmarks/INST_36x36 1200 sudoku_acs 1
# ./parallel.sh test 1200 sudoku_lsc 1
benchmarks=$1
time_t=$2
solver=$3
seed=$4

function parallel() {
    for file in $@/*;do 
        index=`basename $file`
        nohup bash run.sh $file $time_t $solver $seed $ > results/$solver/$@/result_$index.log 2>&1 &
    done
}

for folder in $benchmarks/*;do
    path=results/$solver/$folder
    if [ ! -d "$path" ];then
        mkdir -p $path
    fi
    parallel $folder
done