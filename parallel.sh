#！/bin/bash
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_ls 1
# ./parallel.sh benchmarks/N-Queens 1000 nqueens_ort 1
# ./parallel.sh benchmarks 1000 sudoku_test 1
# ./parallel.sh benchmarks 1000 sudoku_ort 1
# ./parallel.sh benchmarks_SAT 1000 sudoku_sat 1
benchmarks=$1
time_t=$2
solver=$3
seed=$4

function parallel() {
    foldername=`basename $@`
    for file in $@/*;do
        if test -f $file;then
            nohup bash run.sh $@ $time_t $solver $seed $ > results/$solver/$foldername/result.log 2>&1 &
            return
        fi
        index=`basename $file`
        nohup bash run.sh $file $time_t $solver $seed $ > results/$solver/$foldername/result_$index.log 2>&1 &
    done
}

for folder in $benchmarks/*;do
    foldername=`basename $folder`
    path=results/$solver/$foldername
    if [ ! -d "$path" ];then
        mkdir -p $path
    fi
    parallel $folder
done