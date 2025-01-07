#！/bin/bash

function classify() {
    for file in $@/*;do
        filename=$(basename $file)
        category=$(echo $filename | sed -ne 's/inst\([0-9]\+x[0-9]\+\)_\([0-9]\+\)_[0-9]\+.txt/\1/p')
        category=INST_$category
        percentage=$(echo $filename | sed -ne 's/inst\([0-9]\+x[0-9]\+\)_\([0-9]\+\)_[0-9]\+.txt/\2/p')
        # percentage=INST_$percentage
        path=benchmarks/$category/$percentage
        if [ ! -d "$path" ];then
            mkdir -p $path
        fi
        mv $file $path
    done
}

classify $1

# echo inst16x16_0_0.txt | sed -ne 's/inst\([0-9]\+x[0-9]\+\)/\1/p'

# dirname=benchmarks/INST_9x9/0
