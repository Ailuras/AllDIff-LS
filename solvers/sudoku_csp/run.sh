#！/bin/bash

function test_all() {
    for file in $@/*; do
        echo --------------------------------------------------
        start=$[$(date +%s%N)/1000000]
        timeout 100 minizinc --solver gecode $file
        end=$[$(date +%s%N)/1000000]
        take=$(( end - start ))
        echo $file : ${take} ms.
    done
}

test_all $1