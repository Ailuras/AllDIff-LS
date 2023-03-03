#！/bin/bash

function get_all() {
    for file in $@/*; do
        python script/gen_sat_formula.py $file ${file/benchmarks/benchmarks_SAT}
    done
}

path=${1/benchmarks/benchmarks_SAT}
if [ ! -d "$path" ];then
    mkdir -p $path
fi
get_all $1