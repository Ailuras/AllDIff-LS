#！/bin/bash
# ./parallel.sh benchmarks/INST_36x36/ 1200 0
time_t=$2
solver=$3
order=$4

index=1
function parallel() {
    for folder in $@/*;do 
        index=`basename $folder`
        # echo $index
        nohup bash run.sh $folder $time_t $solver > results/result_$order/result_$index.log 2>&1 &
    done
}

if [ ! -d results/result_$order ];then
  mkdir results/result_$order
fi

parallel $1