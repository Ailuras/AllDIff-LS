#！/bin/bash
# ./parallel.sh benchmarks/INST_36x36/ 1200 sudoku_acs 6
# ./parallel.sh test/ 1200 sudoku_acs 6
time_t=$2
solver=$3
order=$4
seed=$5

function parallel() {
    for folder in $@/*;do 
        index=`basename $folder`
        path=results/$solver/result_$order
        if [ ! -d "$path" ];then
            mkdir -p $path
        fi
        nohup bash run.sh $folder $time_t $solver $ > $path/result_$index.log 2>&1 &
    done
}

parallel $1