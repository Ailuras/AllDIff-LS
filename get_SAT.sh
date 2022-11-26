#！/bin/bash

function get_all() {
    for file in $@/*; do
        python gen_sudoku_formula.py $file
    done
}

get_all $1