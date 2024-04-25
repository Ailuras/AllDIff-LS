#！/bin/bash
benchmarks=$1
time_t=$2
solver=$3
seed=$4

# ./parallel.sh ../benchmarks/MOLS 1000 test 1

function parallel() {
    foldername=`basename $@`
    for file in $@/*;do
        if test -f $file;then
            nohup bash run.sh $@ $time_t $solver $seed $ > ../Results/$solver/$foldername/result.log 2>&1 &
            return
        fi
        index=`basename $file`
        nohup bash run.sh $file $time_t $solver $seed $ > ../Results/$solver/$foldername/result_$index.log 2>&1 &
    done
}

for folder in $benchmarks/*; do
    if [ -d "$folder" ]; then
        foldername=`basename $folder`
        path=../Results/$solver/$foldername
        if [ ! -d "$path" ]; then
            mkdir -p $path
        fi
        parallel $folder
    elif [ -f "$folder" ]; then
        foldername=`basename $benchmarks`
        path=../Results/$solver/$foldername
        if [ ! -d "$path" ]; then
            mkdir -p $path
        fi
        parallel $benchmarks
        break
    fi
done