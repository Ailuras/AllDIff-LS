#！/bin/bash
# ./parallel_SAT.sh benchmarks

function parallel() {
    for order_folder in $@/*;do
        for prop_folder in $order_folder/*;do
            nohup bash get_SAT.sh $prop_folder > out.log 2>&1 &
        done
    done
}

parallel $1