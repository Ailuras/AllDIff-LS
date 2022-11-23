#！/bin/bash
# ./parallel.sh benchmarks 1000 sudoku_acs 1
# ./parallel.sh benchmarks_part_CP 1000 sudoku_csp 1
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