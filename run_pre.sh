# ./solvers/sudoku_acs/sudoku_acs --alg 0 --blank --order 7 --verbose > out.log

index=$[ $1 + 5 ]
path=benchmarks/INST_$[$2 * $2]x$[$2 * $2]/100/inst$[$2 * $2]x$[$2 * $2]_100_
for((i=$1;i<$index;i++));do
    file=benchmarks/INST_$[$2 * $2]x$[$2 * $2]/0/inst$[$2 * $2]x$[$2 * $2]_0_$i.txt
    ./solvers/sudoku_lsc/sudoku_gen $file $i 1000 > $path$i.txt
    # line=`cat $path$i.txt | wc -l`
    # while [ $line -lt 10 ]; do
    #     ./solvers/sudoku_lsc/sudoku_lsc --alg 0 --blank --order $2 --verbose > $path$i.txt
    #     line=`cat $path$i.txt | wc -l`
    # done
    # sed -i 's/[-+|]//g; s/^ //g' $path$i.txt
    # sed -i '/^\s*$/d' $path$i.txt
    # sed -i "1c $2" $path$i.txt
    # sed -i '1a 1' $path$i.txt
    # sed -i '$d' $path$i.txt

    sed -i '1i 1' $path$i.txt
    sed -i "1i $2" $path$i.txt
done