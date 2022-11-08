for file in $1/*; do
    echo --------------------------------------------------
    start=$[$(date +%s%N)/1000000]
    ./sudoku_acs/sudokusolver --alg $3 --file $file --timeout $2
    end=$[$(date +%s%N)/1000000]
    take=$(( end - start ))
    echo $file : ${take} ms.
done