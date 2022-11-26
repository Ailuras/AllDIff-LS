#！/bin/bash

function parallel() {
    for prop_folder in $@/*;do
        nohup bash get_SAT.sh $prop_folder > out.log 2>&1 &
    done
}

parallel $1