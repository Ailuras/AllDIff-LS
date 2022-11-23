#！/bin/bash

function parallel() {
    for order_folder in $@/*;do
        for prop_folder in $order_folder/*;do
            folder=$(basename $prop_folder)
            mzn=$(echo $folder | sed -ne 's/LSC_\([0-9]\+_[0-9]\+\)/\1/p')
            result=$mzn.log
            nohup bash run.sh $prop_folder > ../results/CP/$result 2>&1 &
        done
    done
}

parallel $1