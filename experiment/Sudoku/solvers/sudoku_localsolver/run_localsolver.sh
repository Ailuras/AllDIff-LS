#!/bin/bash
input_file=$1
time_t=$2

while read -r file_path; do
    echo --------------------------------------------------
    start=$(date +%s%3N)
    timeout $time_t python solvers/sudoku_localsolver/sudoku.py $file_path
    end=$(date +%s%3N)
    take=$((end-start))
    echo $file_path : ${take} ms.
done < "$input_file"